#include "Lidar.h"

using namespace std;

Lidar::Lidar(int pivot_pin, int sensor_pin){
    _pivot = ServoM(pivot_pin);
    _sensor = IRSensor(sensor_pin);
}

Lidar::~Lidar(){
    cout << "\tdest Lidar" << endl;
}

void Lidar::update(){
    // TODO
}

void Lidar::cycle(){
    // TODO
}