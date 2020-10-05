#ifndef IRSENSOR_H
#define IRSENSOR_H

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

#include <wiringPi.h>

class IRSensor
{
    protected:
        int pin_;       // pin où est branché le sensor
        int address_;    // adresse où est branché le sensor
        int fd_;        // file handler du sensor

    public:
        IRSensor(int addr; int nb_pin);
        ~IRSensor();

        const float& measure() const;
};

#endif