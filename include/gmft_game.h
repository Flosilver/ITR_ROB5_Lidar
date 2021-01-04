#ifndef __GMFT_GAME_H__
#define __GMFT_GAME_H__

#include "lidar.h"
#include "referee.h"
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

class GMFTGame
{
    std::shared_ptr<Lidar> lidar_;
    std::shared_ptr<Referee> referee_;
    std::thread game_thread_;
    std::mutex game_mtx_;
    std::atomic<bool> is_playing_;
    float player_angle_;
    float opening_;
    float seuil_;
    float victory_thresh_;

public:
    GMFTGame(std::shared_ptr<Lidar> lidar, std::shared_ptr<Referee> referee, float player_angle, float opening, float seuil, float victory_thresh=0.08);
    ~GMFTGame();

    std::shared_ptr<const Lidar> lidar() const { return lidar_; }
    std::shared_ptr<Lidar> lidar() { return lidar_; }

    std::shared_ptr<const Referee> referee() const { return referee_; }
    std::shared_ptr<Referee> referee() { return referee_; }

    bool isPlaying() const { return is_playing_.load(); }

    void play();
    void stop();

private:
    void gamePlay();
    void extract(std::list<Lidar::Measure>& scan, float& pos_j1, float& pos_j2);
};

#endif