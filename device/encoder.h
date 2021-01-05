#ifndef __IR_SENSOR_H__
#define __IR_SENSOR_H__

#include <linux/fs.h>

// Pin number
#define IRQ_PIN_1 22
#define IRQ_PIN_2 23

// Struct used to handle interruptions
typedef struct encoder_data
{
    char* label; // The interruption name.
    int irq_pin; // The interruption pin.
    long* count; // The shaft position in increments.
    unsigned int irq; // The interruption identifier.
} encoder_data_t;

// Union used to easily convert long type to an array of bytes.
typedef union float_converter {
    long count;
    char count_byte[sizeof(long)];
} float_converter_t;

#endif // __IR_SENSOR_H__
