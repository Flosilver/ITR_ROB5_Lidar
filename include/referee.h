#ifndef __REFEREE_H__
#define __REFEREE_H__

#include "camera_pan_tilt.h"
#include <mutex>
#include <chrono>

enum target{R_TARGET, L_TARGET};

class Referee : public CameraPanTilt
{
    float player_angle_;
    int target_;
    cv::Mat targets_frame_[2];
    std::atomic<int> lost_;
    std::mutex treatment_mtx_;
    double diff_threshold_;
    size_t lose_thres_;

public:
    Referee(int capture_id, int pan_pin, int tilt_pin, float p_angle, double threshold, size_t lose_thres);
    ~Referee();

    void goToTarget(int t);
    int has_lost() const { return lost_.load(); }

private:
    void treatment(const cv::Mat& frame);
};

#endif
