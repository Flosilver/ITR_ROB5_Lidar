#ifndef __CAMERA_PAN_TILT_H__
#define __CAMERA_PAN_TILT_H__

#include <atomic>
#include <memory>
#include <opencv2/opencv.hpp>
#include <thread>
#include "servo_motor.h"

class CameraPanTilt
{
    std::unique_ptr<ServoMotor> pan_;
    std::unique_ptr<ServoMotor> tilt_;
    cv::Mat frame_;
    std::thread frame_thread_;
    cv::VideoCapture capture_;
    std::atomic<bool> keep_displaying_;

public:
    CameraPanTilt(int capture_id, int pan_pin, int tilt_pin);
    ~CameraPanTilt();

    ServoMotor& pan() { return *pan_; }
    const ServoMotor& pan() const { return *pan_; }

    ServoMotor& tilt() { return *tilt_; }
    const ServoMotor& tilt() const { return *tilt_; }

    cv::Mat& frame() { return frame_; }
    const cv::Mat& frame() const { return frame_; }

    void rotatePan(float angle);
    void rotateTilt(float angle);

    void display();

    void stopDisplay();

private:
    void displayTask();
};

#endif // __CAMERA_PAN_TILT_H__
