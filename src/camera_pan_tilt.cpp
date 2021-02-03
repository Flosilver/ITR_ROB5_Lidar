#include "camera_pan_tilt.h"
#include <cassert>
#include <functional>

CameraPanTilt::CameraPanTilt(int capture_id, int pan_pin, int tilt_pin) :
    pan_(),
    tilt_(),
    frame_(),
    frame_thread_(),
    capture_(capture_id),
    keep_displaying_(false)
{
    assert(capture_.isOpened());
    pan_ = std::move(std::unique_ptr<ServoMotor>(new ServoMotor(pan_pin)));
    tilt_ = std::move(std::unique_ptr<ServoMotor>(new ServoMotor(tilt_pin)));
}

CameraPanTilt::~CameraPanTilt()
{
    stopDisplay();
    capture_.release();
}

void CameraPanTilt::rotatePan(float angle)
{
    pan_->rotate(angle);
    pan_->stop();
}

void CameraPanTilt::rotateTilt(float angle)
{
    tilt_->rotate(angle);
    tilt_->stop();
}

void CameraPanTilt::display()
{
    if (!keep_displaying_.load())
    {
        keep_displaying_.store(true);
        auto task = std::bind(&CameraPanTilt::displayTask, this);
        frame_thread_ = std::thread(task);
    }
}

void CameraPanTilt::displayTask()
{
    if (capture_.isOpened())
    {
        bool is_open(true);
        while (keep_displaying_.load() && is_open)
        {
            if (capture_.read(frame_))
            {
                cv::imshow("Camera", frame_);
                process(frame_);
            }
            is_open = cv::waitKey(33) != 27;
        }
        cv::destroyWindow("Camera");
    }
    keep_displaying_.store(false);
}

void CameraPanTilt::stopDisplay()
{
    // Break the thread infinite loop and wait for it to finish.
    keep_displaying_.store(false);
    if (frame_thread_.joinable()) { frame_thread_.join(); }
}
