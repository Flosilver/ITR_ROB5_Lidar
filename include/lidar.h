#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <string>
#include <vector>

#include "ir_sensor.h"
#include "motor_ctrl.h"



class Lidar
{
    std::shared_ptr<MotorCtrl> pivot_;
    std::shared_ptr<IRSensor> sensor_;
    float left_edge_;
    float right_edge_;

public:
    Lidar(std::shared_ptr<MotorCtrl> pivot, std::shared_ptr<IRSensor> sensor, float l_edge = 1, float r_edge = -1);
    ~Lidar();

    struct Measure
    {
        float pos;      // Angular position of the measure in radians
        float meas;     // Distance measured between the lidar and the measure in meters
    };

    std::vector<Measure> update(float increment=0.1, int sleep=100);

private:
};

#endif
