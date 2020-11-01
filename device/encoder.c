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
MODULE_DESCRIPTION("Pilote pour récupération Interruptions sur Raspberry Pi");

#define DEVICE_NAME "encoder" ///< The device will appear at /dev/encoder using this value
#define CLASS_NAME "encoder" ///< The device class -- this is a character device driv

static int Major;

static struct class* encoder_class = NULL; ///< The device-driver class struct pointer
static struct device* encoder_device = NULL; ///< The device-driver device struct pointer

// Les variables globales et les fonctions sont déclarées statiques afin qu'elles ne soient visibles
// que dans le code de ce fichier. On évite ainsi toute ambiguïté avec autres variables du noyau
// qui partage le même segment de mémoire.

// Adresse de base pour les registres correspondant aux GPIO de la Raspberry Pi :
static volatile unsigned* gpio;

// Valeur correspondant à la position du codeur :
static long encoder_count = 0;

// Déclaration des numéros de pins comme paramètres du module :
static int irq_pin_1 = IRQ_PIN_1;
module_param(irq_pin_1, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

// Déclaration des structures de données utilisées pour traiter les interruptions :
static encoder_data_t data_1;

static struct file_operations encoder_fops = {
    .owner = THIS_MODULE,
    .open = encoder_open,
    .release = encoder_release,
    .read = encoder_read,
    .write = encoder_write,
};

//-------------------------------------
// Fonctions gérant les interruptions :
//-------------------------------------

// Fonction appelée à chaque interruption :
static irqreturn_t encoder_irq_handler(int irq, void* dev_id)
{
    int pin2;
    // Interprétation du pointeur vers les données de l'interruption :
    encoder_data_t* data = (encoder_data_t*)dev_id;
    INP_GPIO(IRQ_PIN_2);
    pin2 = GET_GPIO(IRQ_PIN_2);
    if (pin2 > 1) // Clockwise rotation
        (*data->count)--;
    else // Counter clockwise rotation
        (*data->count)++;
    printk("count: %ld, pin2: %d\n", *data->count, pin2);
    return IRQ_HANDLED;
}

// Fonction pour allouer les pins :
static void setup_irq_pin(encoder_data_t* data)
{
    // Initialisations propre à la Raspberry Pi :
    INP_GPIO(data->irq_pin);
    SET_GPIO_ALT(data->irq_pin, 0);

    // Allocation de l'interruption :
    gpio_request_one(data->irq_pin, GPIOF_IN, data->label); // Réservation système en input

    data->irq = gpio_to_irq(data->irq_pin); // Recherche du numéro d'interruptionà partir du numéro de pin

    if (request_any_context_irq(data->irq,
                                encoder_irq_handler,
                                IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                                THIS_MODULE->name,
                                data) >= 0)
        printk(KERN_INFO "%s: interruption \"%s\" allocated on line %u\n", THIS_MODULE->name, data->label, data->irq);
}

// Fonction pour libérer les pins :
static void free_encoder_pins(encoder_data_t* data)
{
    free_irq(data->irq, data);
    gpio_free(data->irq_pin);
}

//-----------------------------
// Fonctions gérant le module :
//-----------------------------

// Fonction appelée au chargement du module :
int init_module(void)
{
    void* gpio_map;
    // Register the driver
    Major = register_chrdev(Major, "encoder", &encoder_fops);
    if (Major < 0)
    {
        printk(KERN_ALERT "Failed to register the device\n");
        return Major;
    }
    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");

    // Translation des adresses pour l'utilisation directe des GPIO de la Raspberry Pi :
    gpio_map = ioremap(0x3F200000, SZ_16K);
    if (gpio_map == NULL)
    {
        printk(KERN_ALERT "%s: ioremap failed !\n", THIS_MODULE->name);
        return -EBUSY;
    }
    gpio = (volatile unsigned*)gpio_map;

    // Remplissage des structures de données utilisées pour traiter les interruptions :
    data_1.label = "irq 1";
    data_1.irq_pin = irq_pin_1;
    data_1.count = &encoder_count;

    // Initialisations propre à la Raspberry Pi :
    INP_GPIO(IRQ_PIN_2);
    // SET_GPIO_ALT(IRQ_PIN_2, 0);

    // Allocation des pins :
    setup_irq_pin(&data_1);
    // enable pull-up on GPIO24&25
    GPIO_PULL = 2;
    // clock on GPIO 22 & 23 (bit 24 & 25 set)
    GPIO_PULLCLK0 = (1 << IRQ_PIN_1) | (1 << IRQ_PIN_2);
    /****** Automatically creating virtual files in /dev ******/
    // Register the device class
    encoder_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(encoder_class))
    { // Check for error and clean up if there is
        unregister_chrdev(Major, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(encoder_class); // Correct way to return an error on a pointer
    }

    // Register the device driver
    encoder_device = device_create(encoder_class, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(encoder_device))
    { // Clean up if there is an error
        class_destroy(encoder_class); // Repeated code but the alternative is goto statements
        unregister_chrdev(Major, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(encoder_device);
    }

    return 0;
}

// Fonction appelée au retrait du module :
void cleanup_module(void)
{
    device_destroy(encoder_class, MKDEV(Major, 0)); // remove the device
    class_unregister(encoder_class); // unregister the device class
    class_destroy(encoder_class); // remove the device class

    // Libération des pins :
    free_encoder_pins(&data_1);
    GPIO_PULL = 0;
    GPIO_PULLCLK0 = 0;

    unregister_chrdev(Major, "encoder");

    printk(KERN_INFO "%s: module removed\n", THIS_MODULE->name);
}

static int encoder_open(struct inode* inode, struct file* file) { return 0; }

static int encoder_release(struct inode* inode, struct file* file) { return 0; }

static ssize_t encoder_read(struct file* file, char* buf, size_t count, loff_t* ppos)
{
    union {
        long count;
        char count_byte[4];
    } converter;
    int result;

    if (count < 4) { return 0; }
    converter.count = encoder_count;
    result = copy_to_user(buf, converter.count_byte, 4);
    printk(KERN_INFO "Copied : %ld, count: %ld", converter.count, encoder_count);
    if (result < 0)
    {
        printk(KERN_ALERT "Failed to copy to user");
        return 0;
    }
    return 4;
}

static ssize_t encoder_write(struct file* file, const char* buf, size_t count, loff_t* ppos) { return 0; }
