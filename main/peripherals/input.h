#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED



#define FC_DX_ASPIRAZIONE       HAP_IN1_PORT        // INPUT 1 # FC DX - ASPIRAZIONE
#define FC_SX_ASPIRAZIONE       HAP_IN2_PORT        // INPUT 2 # FC SX - ASPIRAZIONE
#define PEDALE_SOFFIO           HAP_IN3_PORT        // INPUT 3 # PEDALE - SOFFIO
#define RECUPERATORE_PIENO      HAP_IN4_PORT        // INPUT 4 # LIVELLO RECUPERATORE PIENO

#define COMANDO_VAPORE_FERRO    HAP_VAP_PORT        // INPUT 5 # FERRO COMANDO VAPORE - 220 VAC



typedef enum {
    INPUT_1 = 0,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
#define INPUT_NUM 5
} input_t;


void input_init(void);
uint8_t input_take_reading(void);
uint8_t input_get(input_t input);
uint16_t input_get_map(void);

#endif
