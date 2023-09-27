#include "lightmodbus/lightmodbus.h"
#include "lightmodbus/slave.h"
#include "peripherals/rs485_display.h"
#include "peripherals/relay.h"
#include "peripherals/input.h"
#include "services/system_time.h"
#include "peripherals/level.h"
#include "peripherals/ptc.h"
#include "peripherals/phase_cut.h"


#define MINION_ADDRESS                  1

#define INPUT_REGISTER_FW_VERSION       0
#define INPUT_REGISTER_INPUTS           1
#define INPUT_REGISTER_LIVELLO1_H2O     2
#define INPUT_REGISTER_LIVELLO2_H2O     3
#define INPUT_REGISTER_PTC1_ADC         4
#define INPUT_REGISTER_PTC2_ADC         5
#define INPUT_REGISTER_PTC1_TEMP        6
#define INPUT_REGISTER_PTC2_TEMP        7

#define INPUT_REGISTER_NUM              8



#define HOLDING_REGISTER_OUTPUTS        0
#define HOLDING_REGISTER_FAN_CONTROL    1

#define HOLDING_REGISTER_NUM            2





static ModbusError register_callback(const ModbusSlave *status, const ModbusRegisterCallbackArgs *args,
                                     ModbusRegisterCallbackResult *result);
static ModbusError exception_callback(const ModbusSlave *minion, uint8_t function, ModbusExceptionCode code);
static ModbusError modbus_static_allocator(ModbusBuffer *buffer, uint16_t size, void *context);


static ModbusSlave minion = {0};


void controller_init(void) {
    modbusSlaveInit(&minion,
                    register_callback,                  // Callback for register operations
                    exception_callback,                 // Callback for handling slave exceptions (optional)
                    modbus_static_allocator,            // Memory allocator for allocating responses
                    modbusSlaveDefaultFunctions,        // Set of supported functions
                    modbusSlaveDefaultFunctionCount     // Number of supported functions
    );
}


void controller_manage(void) {
    static timestamp_t rs485_ts = 0;

    if (is_expired(rs485_ts, get_millis(), 5)) {
        uint8_t buffer[256] = {0};
        int     len         = rs485_display_read_rx_buffer(buffer);

        if (len > 1) {
            ModbusErrorInfo err;
            err = modbusParseRequestRTU(&minion, MINION_ADDRESS, buffer, len);

            if (modbusIsOk(err)) {
                rs485_display_clean_rx_buffer();
                size_t rlen = modbusSlaveGetResponseLength(&minion);
                if (rlen > 0) {
                    rs485_display_sync_write((uint8_t *)modbusSlaveGetResponse(&minion), rlen);
                } else {
                    // ESP_LOGD(TAG, "Empty response");
                }
            } else if (err.error != MODBUS_ERROR_ADDRESS && err.error != MODBUS_ERROR_CRC) {
                // ESP_LOGW(TAG, "Invalid request with source %i and error %i", err.source, err.error);
                // ESP_LOG_BUFFER_HEX(TAG, buffer, len);
            }
        }

        rs485_ts = get_millis();
    }
}


static ModbusError register_callback(const ModbusSlave *minion, const ModbusRegisterCallbackArgs *args,
                                     ModbusRegisterCallbackResult *result) {
    switch (args->query) {
        // R/W access check
        case MODBUS_REGQ_R_CHECK:
        case MODBUS_REGQ_W_CHECK:
            // If result->exceptionCode of a read/write access query is not MODBUS_EXCEP_NONE,
            // an exception is reported by the slave. If result->exceptionCode is not set,
            // the behavior is undefined.
            switch (args->type) {
                case MODBUS_INPUT_REGISTER:
                    result->exceptionCode = args->query == MODBUS_REGQ_R_CHECK && args->index <= INPUT_REGISTER_NUM ? MODBUS_EXCEP_NONE : MODBUS_EXCEP_ILLEGAL_VALUE;
                    break;
                case MODBUS_HOLDING_REGISTER:
                    result->exceptionCode = args->index <= HOLDING_REGISTER_NUM ? MODBUS_EXCEP_NONE : MODBUS_EXCEP_ILLEGAL_VALUE;
                    break;
                default:
                    result->exceptionCode = MODBUS_EXCEP_ILLEGAL_VALUE;
                    break;
            }
            break;

        // Read register
        case MODBUS_REGQ_R:
            switch (args->type) {
                case MODBUS_HOLDING_REGISTER:
                    switch (args->index) {
                        case HOLDING_REGISTER_OUTPUTS:
                            result->value = relay_read_map();
                            break;
                    }
                    break;
                    
                case MODBUS_INPUT_REGISTER: {
                    switch (args->index) {
                        case INPUT_REGISTER_INPUTS:
                            result->value = input_get_map();
                            break;
                        case INPUT_REGISTER_LIVELLO1_H2O:
                            result->value = level_get_adc_value(LEVEL_1);
                            break;
                        case INPUT_REGISTER_LIVELLO2_H2O:
                            result->value = level_get_adc_value(LEVEL_2);
                            break;
                        case INPUT_REGISTER_PTC1_ADC:
                            result->value = ptc_get_adc_value(PTC_1);
                            break;
                        case INPUT_REGISTER_PTC2_ADC:
                            result->value = ptc_get_adc_value(PTC_2);
                            break;
                        case INPUT_REGISTER_PTC1_TEMP:
                            result->value = ptc_get_temperature(PTC_1);
                            break;
                        case INPUT_REGISTER_PTC2_TEMP:
                            result->value = ptc_get_temperature(PTC_2);
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
            break;

        // Write register
        case MODBUS_REGQ_W:
            switch (args->type) {
                case MODBUS_HOLDING_REGISTER: {
                    switch (args->index) {
                        case HOLDING_REGISTER_OUTPUTS:
                            relay_update(RELAY_1, (args->value & 0x01) > 0);
                            relay_update(RELAY_2, (args->value & 0x02) > 0);
                            relay_update(RELAY_3, (args->value & 0x04) > 0);
                            relay_update(RELAY_4, (args->value & 0x08) > 0);
                            relay_update(RELAY_5, (args->value & 0x10) > 0);
                            relay_update(RELAY_6, (args->value & 0x20) > 0);
                            relay_update(RELAY_7, (args->value & 0x40) > 0);
                            break;
                        case HOLDING_REGISTER_FAN_CONTROL: {
                            uint8_t percentage_aspiration = args->value & 0xFF;
                            uint8_t percentage_blow = (args->value >> 8) & 0xFF;
                            phase_cut_set_percentage(PHASE_CUT_ASPIRATION, percentage_aspiration);
                            phase_cut_set_percentage(PHASE_CUT_BLOW, percentage_blow);
                            break;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
            break;
    }

    // Always return MODBUS_OK
    return MODBUS_OK;
}


/*
    Exception callback for printing out exceptions
*/
static ModbusError exception_callback(const ModbusSlave *minion, uint8_t function, ModbusExceptionCode code) {
    // printf("Slave reports an exception %s (function %d)\n",modbusExceptionCodeStr(code),function);

    // Always return MODBUS_OK
    return MODBUS_OK;
}


// !!! This only works if the allocator is used exclusively by a single instance of ModbusMaster or ModbusSlave !!!
static ModbusError modbus_static_allocator(ModbusBuffer *buffer, uint16_t size, void *context) {
    (void)context;
    static uint8_t request[256];

    if (!size) {
        // Pretend we're freeing the buffer
        buffer->data = NULL;
        return MODBUS_OK;
    } else {
        if (size > 256) {
            // Requested size is too big, return allocation error
            buffer->data = NULL;
            return MODBUS_ERROR_ALLOC;
        } else {
            // Return a pointer to our buffer
            buffer->data = request;
            return MODBUS_OK;
        }
    }
}
