#ifndef IRSENSOR_H
#define IRSENSOR_H

#include <stdio.h>
#include <unistd.h>
#include <iostream.h>
#include <stdlib.h>

#include <wiringPi.h>

class IRSensor
{
    protected:
        int _pin;

    public:
        IRSensor(int nb_pin);
        ~IRSensor();

        const float& measure() const;
};

#endif