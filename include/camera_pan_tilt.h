#ifndef __CAMERA_PAN_TILT_H__
#define __CAMERA_PAN_TILT_H__

#include <atomic>
#include <memory>
#include <opencv2/opencv.hpp>
#include <thread>
#include "servo_motor.h"

class CameraPanTilt
{
    std::unique_ptr<ServoMotor> pan_; ///< Pointer to the Pan servomotor
    std::unique_ptr<ServoMotor> tilt_; ///< pointer to the Tilt servomotor
    cv::Mat frame_; ///< Current frame of the video
    std::thread frame_thread_; ///< Video reading thread. Can launch a treatment
    cv::VideoCapture capture_; ///< Opencv VideoCapure
    std::atomic<bool> keep_displaying_; ///< Display flag

public:
    /**
     * Construct a CameraPanTilt.
     * 
     * @param capture_id VideoCapture argument from opencv.
     * @param pan_pin Pan servomotor's pin number.
     * @param tilt_pin Tilt servomotor's pin number.
     */
    CameraPanTilt(int capture_id, int pan_pin, int tilt_pin);

    ~CameraPanTilt();

    /**
     * Get the pan servomotor of the Camera.
     *
     * @return the pan servomotor of the Camera.
     */
    ServoMotor& pan() { return *pan_; }
    /**
     * @overload pan()
     */
    const ServoMotor& pan() const { return *pan_; }

    /**
     * Get the tilt servomotor of the Camera
     * 
     * @return the tilt servomotor of the camera.
     */
    ServoMotor& tilt() { return *tilt_; }
    /**
     * @overload tilt()
     */
    const ServoMotor& tilt() const { return *tilt_; }

    /**
     * Get the current frame of the video.
     * 
     * @return the current frame if the video.
     */
    cv::Mat& frame() { return frame_; }
    /**
     * @overload frame()
     */
    const cv::Mat& frame() const { return frame_; }

    /**
     * Rotate the pan servomotor to the desired angle.
     * 
     * @param angle The desired angle.
     */
    void rotatePan(float angle);
    /**
     * Rotate the tilt servomotor to the desired angle.
     * 
     * @param angle The desired angle.
     */
    void rotateTilt(float angle);

    /**
     * Strat the thread of the video and the possible treatment.
     */
    void display();
    /**
     * Stop the thread of the video and the possible treatment.
     */
    void stopDisplay();

private:
    /**
     * The video thread function.
     */
    void displayTask();

    virtual void treatment(const cv::Mat& frame) {}
};

#endif // __CAMERA_PAN_TILT_H__
