#include "referee.h"
#include <iostream>

#define WIN_1 "Treatment player 0"
#define WIN_2 "Treatment player 1"

Referee::Referee(int capture_id, int pan_pin, int tilt_pin, float p_angle, double threshold, size_t lose_thres) :
    CameraPanTilt(capture_id, pan_pin, tilt_pin),
    player_angle_(p_angle),
    target_(Target::NONE),
    do_treatment_(true),
    diff_threshold_(threshold),
    lose_thres_(lose_thres),
    treatment_cooldown_(500)
{
    rotatePan(0.0F);
    auto now(std::chrono::system_clock::now());
    last_treatment = now;
}

Referee::~Referee()
{
    cv::destroyWindow(WIN_1);
    cv::destroyWindow(WIN_2);
}

void Referee::goToTarget(Target t)
{
    do_treatment_.store(false); // Prevent treament while the motor is moving
    // Move the motor to the given target
    switch (t)
    {
    case Target::R_TARGET:
        target_ = Target::R_TARGET;
        rotatePan(-player_angle_);
        targets_frame_[target_] = cv::Mat();
        break;

    case Target::L_TARGET:
        target_ = Target::L_TARGET;
        rotatePan(player_angle_);
        targets_frame_[target_] = cv::Mat();
        break;

    default:
        std::cout << "***WARNING: Referee::goToTarget(int t): wrong target given, must be either 0 or 1. Given: " << t
                  << "\n";
        break;
    }
    // Wait for the motor to stabilize
    std::chrono::milliseconds delay(1200);
    std::this_thread::sleep_for(delay);

    do_treatment_.store(true); // Re-enable the image processing
}

void Referee::process(const cv::Mat& frame)
{
    if (target_ == NONE || !do_treatment_.load()) { return; }
    // Cooldown between two checks
    auto now(std::chrono::system_clock::now());
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_treatment) < treatment_cooldown_) { return; }
    last_treatment = now;

    cv::Mat treatment_frame;
    cv::cvtColor(frame, treatment_frame, CV_BGR2GRAY);

    // Case of first observation
    if (targets_frame_[target_].empty())
    {
        targets_frame_[target_] = treatment_frame.clone();
        return;
    }

    // Treatment: subtract and binarize
    cv::Mat diff = targets_frame_[target_] - treatment_frame;
    cv::Mat binary;
    cv::threshold(diff, binary, diff_threshold_, 255, CV_THRESH_BINARY);

    // Visualization
    if (target_ == 0) cv::imshow(WIN_1, binary);
    if (target_ == 1) cv::imshow(WIN_2, binary);

    // Loser detection
    int diff_px_nb(0);
    for (int i = 0; i < binary.rows; ++i)
        for (int j = 0; j < binary.cols; ++j)
        {
            int px_value = binary.at<int>(i, j);
            if (px_value > 0) ++diff_px_nb;
        }
    if (lose_thres_ < diff_px_nb) { std::cout << "Joueur " << target_ << " a bougé! Retourne au point de départ!\n"; }

    // Update the target frame
    targets_frame_[target_] = treatment_frame;
}
