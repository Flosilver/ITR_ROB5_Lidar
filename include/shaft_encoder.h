#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <string>

class ShaftEncoder
{
    int file_desc_; ///< File descriptor to the device.
    long offset_; ///< Offset from the device's home (in increments).
    long cpr_; ///< Count per revolution (in increments).

public:
    /**
     * Construct a new Shaft Encoder.
     *
     * Open the device. The file descriptor is only released upon deletion.
     *
     * @param device The path leading to the device.
     * @param cpr The count per revolution (in increments).
     * @param offset The offset from the device's home (in increments).
     */
    ShaftEncoder(std::string device, long cpr, long offset = 0);

    /**
     * Destroy the Shaft Encoder
     *
     * Release the file descriptor associated with the device.
     */
    ~ShaftEncoder();

    /**
     * Get the count per revolution (in increments).
     *
     * @return the count per revolution (in increments).
     */
    long cpr() const { return cpr_; }

    /**
     * Get the offset from the device's home (in increments).
     *
     * The device's home is where the measure yields 0.
     *
     * @return the offset from the device's home (in increments).
     */
    long offset() const { return offset_; }

    /**
     * Set the offset from the device's home (in increments).
     *
     * The device's home is where the measure yields 0.
     *
     * @return the offset from the device's home (in increments).
     */
    void setOffset(long offset) { offset_ = offset; }

    /**
     * Measures the position of the shaft in increments.
     *
     * @return The position of the shaft in increments.
     * @throw std::logic_error If the device failed to provide data.
     */
    long measureIncrements() const;

    /**
     * Measures the position of the shaft in radians.
     *
     * @return The position of the shaft in radians.
     * @throw std::logic_error If the device failed to provide data.
     */
    float measurePosition() const;
};
#endif
