#include "referee.h"
#include <iostream>

#define WIN_1 "Treatment player 0"
#define WIN_2 "Treatment player 1"

Referee::Referee(int capture_id, int pan_pin, int tilt_pin, float p_angle, double threshold, size_t lose_thres) :
    CameraPanTilt(capture_id, pan_pin, tilt_pin),
    player_angle_(p_angle),
    target_(-1),
    do_treatment_(true),
    diff_threshold_(threshold),
    lose_thres_(lose_thres),
    treatment_cooldown_(500)
{
    //cv::namedWindow(WIN_1, CV_WINDOW_AUTOSIZE);
    //cv::namedWindow(WIN_2, CV_WINDOW_AUTOSIZE);
    rotatePan(0);
    //rotateTilt(0);
    auto now(std::chrono::system_clock::now());
    last_treatment = now;
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
    do_treatment_.store(false);
    std::chrono::milliseconds delay(1200);
    switch (t)
    {
    case target::R_TARGET:
        target_ = target::R_TARGET;
        rotatePan(-player_angle_);
        targets_frame_[target_] = cv::Mat();
        break;

    case target::L_TARGET:
        target_ = target::L_TARGET;
        rotatePan(player_angle_);
        targets_frame_[target_] = cv::Mat();
        break;

    default:
        std::cout << "***WARNING: Referee::goToTarget(int t): wrong target given, must be either 0 or 1. Given: " << t
                  << "\n";
        break;
    }
    std::this_thread::sleep_for(delay);
    do_treatment_.store(true);
}

void Referee::treatment(const cv::Mat& frame)
{
    if (target_ == -1 or !do_treatment_.load()) { return; }

    auto now(std::chrono::system_clock::now());
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_treatment) <  treatment_cooldown_) { return; }
    last_treatment = now;

    cv::Mat treatment_frame;
    cv::cvtColor(frame, treatment_frame, CV_BGR2GRAY);

    // case of first observation
    if (targets_frame_[target_].empty())
    {
        targets_frame_[target_] = treatment_frame.clone();
        return;
    }

    // treatment: subtraction + binarize
    cv::Mat diff = targets_frame_[target_] - treatment_frame;
    cv::Mat binary;
    cv::threshold(diff, binary, diff_threshold_, 255, CV_THRESH_BINARY);

    // test phase
    if (target_ == 0) cv::imshow(WIN_1, binary);
    if (target_ == 1) cv::imshow(WIN_2, binary);

    // loser detection
    int diff_px_nb(0);
    for (int i = 0; i < binary.rows; ++i)
        for (int j = 0; j < binary.cols; ++j)
        {
            int px_value = binary.at<int>(i, j);
            if (px_value > 0) ++diff_px_nb;
        }
    //std::cout << "diff px nb = " << diff_px_nb << std::endl;
    if (lose_thres_ < diff_px_nb) 
    {
        std::cout << "Player " << target_ << " a bougé! Retourne au point de départ!\n";
    }

    // MAJ target_frame
    targets_frame_[target_] = treatment_frame.clone();
    treatment_frame.deallocate();
}
