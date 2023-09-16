#ifndef RS485_DISPLAY_H_INCLUDED
#define RS485_DISPLAY_H_INCLUDED

#include <stdint.h>



void rs485_display_init(void);
int rs485_display_sync_write(uint8_t *data, int len);
int rs485_display_async_write(uint8_t *data, int len);
int rs485_display_read_rx_buffer(uint8_t *buffer);
void rs485_display_clean_rx_buffer();
void rs485_display_manage(void);


#endif  /* UART_DRIVER_H_INCLUDED */
