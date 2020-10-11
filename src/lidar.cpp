#include "lidar.h"
#include <iostream>

using namespace std;

Lidar::Lidar(int pivot_pin, int sensor_pin) : pivot_(pivot_pin), sensor_(4, sensor_pin) {}

Lidar::~Lidar() { cout << "\tdest Lidar\n"; }

void Lidar::update()
{
    // TODO
}

void Lidar::cycle()
{
    // TODO
}
