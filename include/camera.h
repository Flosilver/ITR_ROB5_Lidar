#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <wiringPi.h>
#include "servo_motor.h"

class Camera
{
public:
    Camera(int pan_pin, int tilt_pin);
    ~Camera();

    void setPanTarget(float angle);
    void setTiltTarget(float angle);

private:
    ServoMotor pan_;
    ServoMotor tilt_;
};

#endif
