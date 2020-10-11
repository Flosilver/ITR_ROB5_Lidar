#ifndef LIDAR_H
#define LIDAR_H

#include "ir_sensor.h"
#include "servo_motor.h"

class Lidar
{
public:
    Lidar(int pivot_pin, int sensor_pin);
    ~Lidar();

    void update();
    void cycle();

private:
    ServoMotor pivot_;
    IRSensor sensor_;
};

#endif
