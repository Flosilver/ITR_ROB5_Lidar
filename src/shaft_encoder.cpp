#include "shaft_encoder.h"
#include <cerrno>
#include <cmath>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

ShaftEncoder::ShaftEncoder(std::string device, long cpr, long offset) : offset_(offset), cpr_(cpr)
{
    file_desc_ = open(device.c_str(), O_RDONLY);
    if (file_desc_ < 0) { throw std::logic_error("Failed to open the device!"); }
}

ShaftEncoder::~ShaftEncoder()
{
    // Close the device iff it has been open.
    if (file_desc_ > 0 && close(file_desc_) == -1)
    {
        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__
                  << ": Failed to close the device: " << std::strerror(errno) << std::endl;
    }
}

long ShaftEncoder::measureIncrements() const
{
    union {
        char bytes[sizeof(long)];
        long value;
    } data; // Easy conversion from bytes to long
    int bytes_amount = read(file_desc_, data.bytes, sizeof(long));
    if (bytes_amount != sizeof(long))
    {
        std::stringstream msg;
        msg << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Invalid read size." << std::endl;
        throw std::logic_error(msg.str());
    }
    return data.value - offset_; // Convert the bytes to a long
}

float ShaftEncoder::measurePosition() const { return measureIncrements() * 2.0 * M_PI / cpr_; }
