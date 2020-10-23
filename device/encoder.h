#ifndef __IR_SENSOR_H__
#define __IR_SENSOR_H__

// Numéros des pins utilisés :
#define IRQ_PIN_1 16
#define IRQ_PIN_1 17

// Structure utilisée pour traiter les interruptions :
typedef struct encoder_data
{
    char* label;
    int irq_pin;
    long count;
    unsigned int irq;
} encoder_data_t;

#endif // __IR_SENSOR_H__
