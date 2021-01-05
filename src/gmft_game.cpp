#include "gmft_game.h"
#include <cmath>
#include <functional>
#include <iostream>
#include <list>

GMFTGame::GMFTGame(std::shared_ptr<Lidar> lidar,
                   std::shared_ptr<Referee> referee,
                   float player_angle,
                   float opening,
                   float moved_threshold,
                   float victory_thresh) :
    lidar_(lidar),
    referee_(referee),
    game_thread_(),
    is_playing_(false),
    player_angle_(player_angle),
    opening_(opening),
    moved_threshold_(moved_threshold),
    victory_threshold_(victory_thresh)
{
    assert(lidar_ != nullptr);
    assert(referee_ != nullptr);
}

GMFTGame::~GMFTGame() { stop(); }

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
    bool check[2] = {false, false};
    std::chrono::milliseconds game_sleep(333);
    // Start utility objects
    lidar_->start();
    referee_->display();

    while (is_playing_.load())
    {
        // Measure the current players positions.
        ir_scan = lidar_->scan();
        extractPositions(ir_scan, current_pos[0], current_pos[1]);
        // Determine who the referee has to check.
        for (int i = 0; i < 2; i++)
        {
            check[i] = old_pos[i] > 0 && current_pos[i] > 0 && old_pos[i] - current_pos[i] > moved_threshold_;
        }
        if (check[0] && check[1])
        {
            Target target = current_pos[0] < current_pos[1] ? Target::R_TARGET : Target::L_TARGET;
            referee_->goToTarget(target);
        }
        else if (check[0])
            referee_->goToTarget(Target::R_TARGET);
        else if (check[1])
            referee_->goToTarget(Target::L_TARGET);
        // Check if a player is near enough, if so she/he wins.
        if (0 < current_pos[0] && 0 < current_pos[1])
        {
            if (current_pos[0] <= victory_threshold_ && current_pos[1] <= victory_threshold_)
            {
                std::cout << "\n-------\nEXAEQUO\n-------\n";
                is_playing_.store(false);
            }
            else if (current_pos[0] <= victory_threshold_)
            {
                std::cout << "\n-------\nPLAYER 0 WINS\n-------\n";
                is_playing_.store(false);
            }
            else if (current_pos[1] <= victory_threshold_)
            {
                std::cout << "\n-------\nPLAYER 1 WINS\n-------\n";
                is_playing_.store(false);
            }
        }
        // Update the last player position.
        old_pos[0] = current_pos[0];
        old_pos[1] = current_pos[1];

        std::this_thread::sleep_for(game_sleep);
    }
    // Stop all utility objects
    referee_->stopDisplay();
    lidar_->stop();
}

void GMFTGame::extractPositions(std::list<Lidar::Measure>& scan, float& p1_pos, float& p2_pos)
{
    float sumR(0.0);
    float sumL(0.0);
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
    // Output the results, the mean distance or -1 if there is no measurement.
    p1_pos = nb_mesR != 0 ? sumR / nb_mesR : -1.;
    p2_pos = nb_mesL != 0 ? sumL / nb_mesL : -1.;
}
