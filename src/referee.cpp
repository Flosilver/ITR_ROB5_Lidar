#include "referee.h"
#include <iostream>

#define WIN_1 "Treatment player 0"
#define WIN_2 "Treatment player 1"

Referee::Referee(int capture_id, int pan_pin, int tilt_pin, float p_angle, double threshold) :
    CameraPanTilt(capture_id, pan_pin, tilt_pin),
    player_angle_(p_angle),
    target_(-1),
    lost_(-1),
    diff_threshold_(threshold)
{
    cv::namedWindow(WIN_1, CV_WINDOW_AUTOSIZE);
    cv::namedWindow(WIN_2, CV_WINDOW_AUTOSIZE);
    rotatePan(0);
}

Referee::~Referee()
{
    targets_frame_[0].deallocate();
    targets_frame_[1].deallocate();
    cv::destroyWindow(WIN_1);
    cv::destroyWindow(WIN_2);
}

void Referee::goToTarget(int t)
{
    std::lock_guard<std::mutex> tracker_lock(treatment_mtx_);
    std::chrono::milliseconds delay(1000);
    switch (t)
    {
    case target::R_TARGET:
        target_ = target::R_TARGET;
        rotatePan(-player_angle_);
        break;

    case target::L_TARGET:
        target_ = target::L_TARGET;
        rotatePan(player_angle_);
        break;

    default:
        std::cout << "***WARNING: Referee::goToTarget(int t): wrong target given, must be either 0 or 1. Given: " << t
                  << "\n";
        break;
    }
    std::this_thread::sleep_for(delay);
}

void Referee::treatment(const cv::Mat& frame)
{
    if (target_ == -1)
    {
        return;
    }
    
    cv::Mat treatment_frame;
    {
        std::lock_guard<std::mutex> tracker_lock(treatment_mtx_);
        cv::cvtColor(frame, treatment_frame, CV_BGR2GRAY);
    }

    // case of first observation
    if (targets_frame_[target_].empty())
    {
        targets_frame_[target_] = treatment_frame.clone();
        return;
    }

    // treatment: subtraction + binarize
    cv::Mat diff = targets_frame_[target_] - treatment_frame;
    diff = cv::threshold(diff, diff, diff_threshold_, 255, CV_THRESH_BINARY);
    
    // test phase
    if (target_ == 0) cv::imshow(WIN_1, diff);
    if (target_ == 1) cv::imshow(WIN_2, diff);

    // winner definition
    // TODO

    // MAJ target_frame
    targets_frame_[target_] = treatment_frame.clone();
    treatment_frame.deallocate();
}