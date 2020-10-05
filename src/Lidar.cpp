#include "Lidar.h"

using namespace std;

Lidar::Lidar(int pivot_pin, int sensor_pin){
    *pivot_ = ServoM(pivot_pin);
    *sensor_ = IRSensor(sensor_pin);
}

Lidar::~Lidar(){
    cout << "\tdest Lidar\n";
}

void Lidar::update(){
    // TODO
}

void Lidar::cycle(){
    // TODO
}