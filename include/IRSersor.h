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
        int pin_;

    public:
        IRSensor(int nb_pin);
        ~IRSensor();

        const float& measure() const;
};

#endif