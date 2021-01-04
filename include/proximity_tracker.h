#ifndef __PROXIMITY_TRACKER_H__
#define __PROXIMITY_TRACKER_H__

#include <memory>
#include <mutex>
#include <thread>
#include "lidar.h"
#include "camera_pan_tilt.h"

class ProximityTracker
{
    bool is_running_; ///< Flag for the thread
    std::thread tracker_thread_; ///< Thread of the track
    std::mutex tracker_mutex_; ///< mutex for flag gestion
    std::shared_ptr<Lidar> lidar_; ///< Pointer to the Lidar
    std::shared_ptr<CameraPanTilt> camera_; ///< Pointer to the CameraPanTilt

public:
    /**
     * Construct a new dummy ProximityTracker.
     */
    ProximityTracker() = default;
    /**
     * Construct a new ProximityTracker.
     * 
     * @param lidar The Lidar pointer
     * @param designator The CameraPanTilt pointer
     */
    ProximityTracker(std::shared_ptr<Lidar> lidar, std::shared_ptr<CameraPanTilt> designator);
    ~ProximityTracker();

    /**
     * Get the Lidar of the ProximityTracker
     * 
     * @return the Lidar of the ProximityTracker
     */
    std::shared_ptr<const Lidar> lidar() const { return lidar_; }
    /**
     * @overload lidar()
     */
    std::shared_ptr<Lidar> lidar() { return lidar_; }

    /**
     * Get the CameraPanTilt of the ProximityTracker.
     * 
     * @return the CameraPanTilt of the ProximityTracker.
     */
    std::shared_ptr<const CameraPanTilt> designator() const { return camera_; }
    /**
     * @overload designator()
     */
    std::shared_ptr<CameraPanTilt> designator() { return camera_; }

    /**
     * Get the boolean flag to know if the thread is running.
     * 
     * @return the boolean flag to know if the thread is running.
     */
    bool isRunning() const { return is_running_; }

    /**
     * Start the thread.
     */
    void start();
    /**
     * Stop the thread.
     */
    void stop();
private:
    /**
     * Thread method of track.
     */
    void track();
};
#endif // __PROXIMITY_TRACKER_H__
