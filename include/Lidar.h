#ifndef LIDAR_H
#define LIDAR_H

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

#include <wiringPi.h>
#include "ServoM.h"
#include "IRSersor.h"

class Lidar
{
    protected:
        ServoM* pivot_;
        IRSensor* sensor_;

    public:
        Lidar(int pivot_pin, int sensor_pin);
        ~Lidar();

        void update();
        void cycle();
};

#endif