#ifndef __REFEREE_H__
#define __REFEREE_H__

#include <chrono>
#include <mutex>
#include "camera_pan_tilt.h"

enum target
{
    R_TARGET,
    L_TARGET
};

class Referee : public CameraPanTilt
{
    float player_angle_; ///< Angular position of the players.
    int target_; ///< Actual target of the referee.
    cv::Mat targets_frame_[2]; ///< Treatment images of each player.
    std::atomic<bool> do_treatment_; ///< Treatment flag.
    double diff_threshold_; ///< Binarisation threshold of the treatment.
    int lose_thres_; ///< Threshold to detect an abnormal movement by a player.
    std::chrono::milliseconds treatment_cooldown_; ///< Cooldown between each treatment process.
    std::chrono::system_clock::time_point last_treatment; ///< Chrono to check the cooldown

public:
    /**
     * Construct a new Referee.
     *
     * @param capture_id VideoCapture argument from opencv.
     * @param pan_pin Pan servomotor's pin number.
     * @param tilt_pin Tilt servomotor's pin number.
     * @param p_angle Angular position of players around the center.
     * @param threshold Binarisation threshold of the treatment (0 < thresold < 255).
     * @param lose_thres Threshold to detect an abnormal movement by a player.
     */
    Referee(int capture_id, int pan_pin, int tilt_pin, float p_angle, double threshold, size_t lose_thres);

    ~Referee();

    /**
     * Change the target attribut of the Referee. 
     * Forbid the treatment process and cause a sleep of 1.2s.
     *
     * @param t The new target.
     **/
    void goToTarget(int t);

private:
    /**
     * Inherited method.
     * Detect targeted player movement.
     * 
     * @param frame The frame of the camera.
     **/
    void treatment(const cv::Mat& frame);
};

#endif
