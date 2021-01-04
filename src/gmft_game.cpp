#include "gmft_game.h"
#include <iostream>
#include <list>
#include <cmath>
#include <functional>

GMFTGame::GMFTGame(std::shared_ptr<Lidar> lidar, std::shared_ptr<Referee> referee, float player_angle, float opening, float seuil, float victory_thresh):
lidar_(lidar),
referee_(referee),
game_thread_(),
game_mtx_(),
is_playing_(false),
player_angle_(player_angle),
opening_(opening),
seuil_(seuil),
victory_thresh_(victory_thresh)
{
    assert(lidar_ != nullptr);
    assert(referee_ != nullptr);
}

GMFTGame::~GMFTGame(){ stop(); }

void GMFTGame::play()
{
    if (!is_playing_.load())
    {
        is_playing_.store(true);
        auto task = std::bind(&GMFTGame::gamePlay, this);
        game_thread_ = std::thread(task);
    }
}

void GMFTGame::stop()
{
    if (is_playing_.load())
    {
        is_playing_.store(false);
        game_thread_.join();
    }
}

void GMFTGame::gamePlay()
{
    std::list<Lidar::Measure> ir_scan;
    float old_pos[2] = {-1., -1.};
    float current_pos[2] = {-1., -1.};

    std::chrono::milliseconds game_sleep(333);

    lidar_->start();
    referee_->display();

    bool check[2] = {false, false};

    while(is_playing_.load())
    {
        ir_scan = lidar_->scan();
        extract(ir_scan, current_pos[0], current_pos[1]);
        
        // Determine who the referee has to check
        for (int i=0 ; i<2 ; i++)
        {
            check[i] = old_pos[i] > 0 && current_pos[i] > 0 && old_pos[i] - current_pos[i] > seuil_;
        }

        if (check[0] && check[1])
        {
            int target = current_pos[0] < current_pos[1] ? 0 : 1;
            referee_->goToTarget(target);
        }
        else if (check[0]) referee_->goToTarget(0);
        else if (check[1]) referee_->goToTarget(1);

        // La suite... TODO
        // A player is near enough -> he wins -> the end
        if (0 < current_pos[0] &&  0 < current_pos[1])
        {
            if (current_pos[0] <= victory_thresh_ && current_pos[1] <= victory_thresh_)
            {
                std::cout << "\n-------\nEXAEQUO\n-------\n";
                is_playing_.store(false);
            }
            else if (current_pos[0] <= victory_thresh_)
            {
                std::cout << "\n-------\nPLAYER 0 WINS\n-------\n";
                is_playing_.store(false);
            }
            else if (current_pos[1] <= victory_thresh_ )
            {
                std::cout << "\n-------\nPLAYER 1 WINS\n-------\n";
                is_playing_.store(false);
            }
        }
        
        // MAJ old_pos
        old_pos[0] = current_pos[0];
        old_pos[1] = current_pos[1];
        
        // game sleep
        std::this_thread::sleep_for(game_sleep);
    }
    referee_->stopDisplay();
    lidar_->stop();
}

void GMFTGame::extract(std::list<Lidar::Measure>& scan, float& pos_j1, float& pos_j2)
{
    float sumR(0.);
    float sumL(0.);
    int nb_mesR(0);
    int nb_mesL(0);

    for (const Lidar::Measure& mes : scan)
    {
        // J1 - Right player
        if (std::abs(mes.orientation + player_angle_) < opening_)
        {
            sumR += mes.distance;
            nb_mesR++;
        }
        // J2 - Left player
        if (std::abs(mes.orientation - player_angle_) < opening_)
        {
            sumL += mes.distance;
            nb_mesL++;
        }
    }

    pos_j1 = nb_mesR != 0 ? sumR/nb_mesR : -1.;
    pos_j2 = nb_mesL != 0 ? sumL/nb_mesL : -1.;
}