#include "encoder.h"
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include "gpio.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Pilote pour récupération Interruptions sur Raspberry Pi");

// Les variables globales et les fonctions sont déclarées statiques afin qu'elles ne soient visibles
// que dans le code de ce fichier. On évite ainsi toute ambiguïté avec autres variables du noyau
// qui partage le même segment de mémoire.

// Adresse de base pour les registres correspondant aux GPIO de la Raspberry Pi :
static volatile unsigned* gpio;

// Valeur correspondant à la position du codeur :
static long encoder_count;

// Déclaration des numéros de pins comme paramètres du module :
static int irq_pin_1 = IRQ_PIN_1;
module_param(irq_pin_1, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

static int irq_pin_2 = IRQ_PIN_2;
module_param(irq_pin_2, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

// Déclaration des structures de données utilisées pour traiter les interruptions :
static encoder_data_t data_1[2];

//-------------------------------------
// Fonctions gérant les interruptions :
//-------------------------------------

// Fonction appelée à chaque interruption :
static irqreturn_t encoder_irq_handler(int irq, void* dev_id)
{
    // Interprétation du pointeur vers les données de l'interruption :
    encoder_data_t* data = (encoder_data_t*)dev_id;

    data->count++;
    printk("irq_rcvd count = %ld \n", *data->count);
    return IRQ_HANDLED;
}

// Fonction pour allouer les pins :
static void setup_irq_pin(encoder_data_t* data)
{
    int i;
    // Initialisations propre à la Raspberry Pi :
    for (i = 0; i < 2; i++)
    {
        INP_GPIO(data->irq_pin[i]);
        SET_GPIO_ALT(data->irq_pin[i], 0);

        // Allocation de l'interruption :
        gpio_request_one(data->irq_pin[i], GPIOF_IN, data->label); // Réservation système en input

        data->irq = gpio_to_irq(data->irq_pin[i]); // Recherche du numéro d'interruptionà partir du numéro de pin

        if (request_any_context_irq(data->irq[i],
                                    encoder_irq_handler,
                                    IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                                    THIS_MODULE->name,
                                    data) >= 0)
            printk(KERN_INFO "%s: interruption \"%s\" allocated on line %u\n",
                   THIS_MODULE->name,
                   data->label[i],
                   data->irq[i]);
    }
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
    int i;

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

    // Allocation des pins :
    setup_irq_pin(&data_1);

    return 0;
}

// Fonction appelée au retrait du module :
void cleanup_module(void)
{
    int i;

    // Libération des pins :
    free_encoder_pins(&data_1);

    printk(KERN_INFO "%s: module removed\n", THIS_MODULE->name);
}
