#ifndef __GMFT_GAME_H__
#define __GMFT_GAME_H__

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include "lidar.h"
#include "referee.h"

class GMFTGame
{
    std::shared_ptr<Lidar> lidar_; ///< The LiDAR used to sens player moving.
    std::shared_ptr<Referee> referee_; ///< The referee that verifies the player is moving.
    std::thread game_thread_; ///< The thread handling the game.
    std::atomic<bool> is_playing_; ///< The flag controlling the thread loop.
    float player_angle_; ///< The angle at which there are the players.
    float opening_; ///< The tolerance on the player position.
    float moved_threshold_; ///< The threshold on the position difference to tell a player has moved.
    float victory_threshold_; ///< The threshold on the player position to tell he/she has won.

public:
    /**
     * Construct a dummy GMFTGame.
     */
    GMFTGame() = default;

    /**
     * Construct a new GMFTGame.
     *
     * @param lidar The LiDAR used to sens player moving.
     * @param referee The referee that verifies the player is moving.
     * @param player_angle The angle at which there are the players symetrical to the center.
     * @param opening The tolerance on the player position.
     * @param moved_threshold The threshold on the position difference to tell a player has moved.
     * @param victory_thresh The threshold on the player position to tell he/she has won.
     */
    GMFTGame(std::shared_ptr<Lidar> lidar,
             std::shared_ptr<Referee> referee,
             float player_angle,
             float opening,
             float seuil,
             float victory_thresh = 0.08);

    /**
     * Destroy the GMFTGame.
     *
     * Stop the thread handling the game.
     */
    ~GMFTGame();

    /**
     * Get the LiDAR used to sens player moving.
     *
     * @return The LiDAR used to sens player moving.
     */
    std::shared_ptr<const Lidar> lidar() const { return lidar_; }
    /**
     * @overload lidar()
     */
    std::shared_ptr<Lidar> lidar() { return lidar_; }

    /**
     * Get the referee that verifies the player is moving.
     *
     * @return The referee that verifies the player is moving.
     */
    std::shared_ptr<const Referee> referee() const { return referee_; }
    /**
     * @overload referee()
     */
    std::shared_ptr<Referee> referee() { return referee_; }

    /**
     * Predicate that tells whether a game is in progress or not.
     *
     * @return Whether a game is in progress or not.
     */
    bool isPlaying() const { return is_playing_.load(); }

    /**
     * Start a new game.
     *
     * If a game has already started, do nothing.
     */
    void play();

    /**
     * Stop the current game.
     *
     * If a game has not already started, do nothing.
     */
    void stop();

private:
    /**
     * Task handling a game instance.
     *
     * Measure the current players positions.
     * Determine who the referee has to check.
     * Check if a player is near enough, if so she/he wins.
     */
    void gamePlay();

    /**
     * Extract the players positions out of a LiDAR scan.
     *
     * @param scan The measurements from the LiDAR.
     * @param p1_pos [out] The player 1 distance to the LiDAR (in meters).
     * @param p2_pos [out] The player 2 distance to the LiDAR (in meters).
     */
    void extractPositions(std::list<Lidar::Measure>& scan, float& p1_pos, float& p2_pos);
};

#endif
