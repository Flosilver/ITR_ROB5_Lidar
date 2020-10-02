#include "Camera.h"

using namespace std;

Camera::Camera(int pan_pin, int tilt_pin){
    _pan = ServoM(pan_pin);
    _tilt = ServoM(tilt_pin);
}

Camera::~Camera(){
    cout << "\tdest Camera" << endl;
}

void Camera::setPanTarget(float angle){
    _pan(angle);
}

void Camera::setTiltTarget(float angle){
    _tilt(angle);
}