#include "camera.h"

using namespace std;

Camera::Camera(int pan_pin, int tilt_pin): pan_(pan_pin), tilt_(tilt_pin) {}

Camera::~Camera() { cout << "\tdest Camera" << endl; }

void Camera::setPanTarget(float angle) { pan_.rotate(angle); }

void Camera::setTiltTarget(float angle) { tilt_.rotate(angle); }
