#include "lidar.h"
#include <iostream>

using namespace std;

Lidar::Lidar(int pivot_pin, int pivot_i2c_addr, int sensor_pin, int sensor_addr) : pivot_(pivot_pin, pivot_i2c_addr), sensor_(sensor_addr, sensor_pin) {}

Lidar::~Lidar() { cout << "\tdest Lidar\n"; }

void Lidar::update()
{
    // TODO
}

void Lidar::cycle()
{
    // TODO
}
