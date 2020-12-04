#ifndef LIDAR_H
#define LIDAR_H

#include "ir_sensor.h"
#include "dc_motor.h"

class Lidar
{
public:
    Lidar(int pivot_pin, int pivot_i2c_addr, int sensor_pin, int sensor_addr);
    ~Lidar();

    void update();
    void cycle();

private:
    DCMotor pivot_;
    IRSensor sensor_;
};

#endif
