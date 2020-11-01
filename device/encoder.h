#ifndef __IR_SENSOR_H__
#define __IR_SENSOR_H__

#include <linux/fs.h>

// Numéros des pins utilisés :
#define IRQ_PIN_1 22
#define IRQ_PIN_2 23

#define TRIGONOMETRIC 0
#define CLOCKWISE 1

// Structure utilisée pour traiter les interruptions :
typedef struct encoder_data
{
    char* label;
    int irq_pin;
    long* count;
    char direction;
    unsigned int irq;
} encoder_data_t;

union float_converter {
    long count;
    char count_byte[4];
};

static int
encoder_open(struct inode* inode, struct file* file);
static int encoder_release(struct inode* inode, struct file* file);
static ssize_t encoder_write(struct file* file, const char* buf, size_t count, loff_t* ppos);
static ssize_t encoder_read(struct file* file, char* buf, size_t count, loff_t* ppos);

#endif // __IR_SENSOR_H__
