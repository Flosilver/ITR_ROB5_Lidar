#ifndef __IR_SENSOR_H__
#define __IR_SENSOR_H__

// Numéros des pins utilisés :
#define IRQ_PIN_1 16

// Définition des macros pour l'utilisation directe des GPIO de la Raspberry Pi :
#define INP_GPIO(g)
#define SET_GPIO_ALT(g, a)
#define OUT_GPIO(g)
#define GET_GPIO(g)

// Structure utilisée pour traiter les interruptions :
typedef struct encoder_data
{
    char* label;
    int irq_pin;
    long* count;
    unsigned int irq;
} encoder_data_t;

#endif // __IR_SENSOR_H__
