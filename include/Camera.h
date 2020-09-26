#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <unistd.h>
#include <iostream.h>
#include <stdlib.h>

#include <wiringPi.h>
#include "ServoM.h"

class Camera
{
    protected:
        ServoM _pan;
        ServoM _tilt;

    public:
        Camera(int pan_pin, int tilt_pin);
        ~Camera();

        void setPanTarget(float angle);
        void setTiltTarget(float angle);
};

#endif