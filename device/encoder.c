#include "encoder.h"
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h> // Header to support the kernel Driver Model
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/init.h> /* module_{init,exit}() */
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h> /* printk() */
#include <linux/module.h> /* modules */
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/slab.h> /* kmalloc()/kfree() */
#include <linux/uaccess.h>
#include <linux/wait.h>
#include "gpio.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Driver for shaft encoder");

#define DEVICE_NAME "encoder" ///< The device will appear at /dev/encoder using this value
#define CLASS_NAME "encoder" ///< The device class -- this is a character device driv

static int Major; ///< The module class identifier
static struct class* encoder_class = NULL; ///< The device-driver class struct pointer
static struct device* encoder_device = NULL; ///< The device-driver device struct pointer
static volatile unsigned* gpio; ///< Pointer to the RPi GPIO registry
static long encoder_count = 0; ///< The position of the shaft (in increments)
static encoder_data_t data_1; ///< Data struct used to handle interruptions

// Pin number as module parameter:
static int irq_pin_1 = IRQ_PIN_1;
static int irq_pin_2 = IRQ_PIN_2;
module_param(irq_pin_1, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
module_param(irq_pin_2, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

//-------------------------------------
//       Interruption handling        :
//-------------------------------------

/**
 * Handle an signal rise interruption on the main pin.
 *
 * @param irq The interruption ID
 * @param dev_id The device data
 * @return IRQ_HANDLED (always)
 */
static irqreturn_t encoder_irq_handler(int irq, void* dev_id)
{
    int pin2;
    // Cast the device ID as encoder data
    encoder_data_t* data = (encoder_data_t*)dev_id;
    // Read the secondary pin
    INP_GPIO(irq_pin_2);
    pin2 = GET_GPIO(irq_pin_2);
    // Deduce the direction and increment the counter
    if (pin2 > 0) // Clockwise rotation
        (*data->count)--;
    else // Counter clockwise rotation
        (*data->count)++;

    return IRQ_HANDLED;
}

/**
 * Setup an interruption from interruption data structure.
 *
 * @param data The interruption data structure.
 */
static void setup_irq_pin(encoder_data_t* data)
{
    // RPi own initiliazation
    INP_GPIO(data->irq_pin);
    SET_GPIO_ALT(data->irq_pin, 0);
    // Allocate the interruption, system reservation as input
    gpio_request_one(data->irq_pin, GPIOF_IN, data->label);
    // Get the interruption identifier from the physical pin
    data->irq = gpio_to_irq(data->irq_pin);
    // Link the interruption to its handler
    if (request_any_context_irq(data->irq, encoder_irq_handler, IRQF_TRIGGER_RISING, THIS_MODULE->name, data) >= 0)
        printk(KERN_INFO "%s: interruption \"%s\" allocated on line %u\n", THIS_MODULE->name, data->label, data->irq);
}

/**
 * Release pin from interruptions.
 *
 * @param data The interruption data structure.
 */
static void free_encoder_pins(encoder_data_t* data)
{
    free_irq(data->irq, data);
    gpio_free(data->irq_pin);
}

//-------------------------------------
//    Device management functions     :
//-------------------------------------

static int encoder_open(struct inode* inode, struct file* file) { return 0; }

static int encoder_release(struct inode* inode, struct file* file) { return 0; }

static ssize_t encoder_read(struct file* file, char* buf, size_t count, loff_t* ppos)
{
    float_converter_t converter;
    int res;
    // Ensure the reader wants the full long value
    if (count < sizeof(long)) { return 0; }
    // Convert the long value to an array of bytes
    converter.count = encoder_count;
    // Transmit that array to the user
    res = copy_to_user(buf, converter.count_byte, sizeof(long));
    if (res < 0)
    {
        printk(KERN_ALERT "Failed to copy to user");
        return 0;
    }
    return sizeof(long);
}

static ssize_t encoder_write(struct file* file, const char* buf, size_t count, loff_t* ppos) { return 0; }

// File operations description for the encoder device
static struct file_operations encoder_fops = {
    .owner = THIS_MODULE,
    .open = encoder_open,
    .release = encoder_release,
    .read = encoder_read,
    .write = encoder_write,
};

//-------------------------------------
//    Module management functions     :
//-------------------------------------

/**
 * Initialize the module.
 *
 * Create a device and prepare for interruption handling.
 *
 * @return 0 on success, anything else is an error number.
 */
int init_module(void)
{
    void* gpio_map;
    int i;

    // Register the device
    Major = register_chrdev(Major, "encoder", &encoder_fops);
    if (Major < 0)
    {
        printk(KERN_ALERT "Failed to register the device\n");
        return Major;
    }

    // Register the device class
    encoder_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(encoder_class))
    { // Check for error and clean up if there is
        unregister_chrdev(Major, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(encoder_class); // Correct way to return an error on a pointer
    }

    // Create the device
    encoder_device = device_create(encoder_class, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(encoder_device))
    { // Clean up if there is an error
        class_destroy(encoder_class); // Repeated code but the alternative is goto statements
        unregister_chrdev(Major, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(encoder_device);
    }

    // Translate the registry address for direct use of RPi GPIO
    gpio_map = ioremap(0x3F200000, SZ_16K);
    if (gpio_map == NULL)
    {
        printk(KERN_ALERT "%s: ioremap failed !\n", THIS_MODULE->name);
        return -EBUSY;
    }
    gpio = (volatile unsigned*)gpio_map;

    // Fill in the interruption data structure
    data_1.label = "irq 1";
    data_1.irq_pin = irq_pin_1;
    data_1.count = &encoder_count;

    // Prepare the main and the secondary pin
    setup_irq_pin(&data_1);
    INP_GPIO(irq_pin_2);
    SET_GPIO_ALT(irq_pin_2, 0);

    // Enable pull-up
    GPIO_PULL = 2;
    for (i = 0; i < 150; i++)
        ; // Wait 150 clock cycles
    GPIO_PULLCLK0 = (1 << irq_pin_1) | (1 << irq_pin_2);

    return 0;
}

/**
 * Clean up the module.
 *
 * Remove the device, deactivate interruptions and the pull up.
 */
void cleanup_module(void)
{
    device_destroy(encoder_class, MKDEV(Major, 0)); // Remove the device
    class_unregister(encoder_class); // Unregister the device class
    class_destroy(encoder_class); // Remove the device class
    unregister_chrdev(Major, "encoder"); // Unregister the device

    // Deactive the interruption on the pins
    free_encoder_pins(&data_1);

    // Deactive the pull up
    GPIO_PULL = 0;
    GPIO_PULLCLK0 = 0;

    printk(KERN_INFO "%s: module removed\n", THIS_MODULE->name);
}
