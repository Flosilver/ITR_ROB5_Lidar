#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include <iostream>

class Arguments
{
public:
    enum class Mode
    {
        INVALID = -1,
        PROXIMITY_TRACKER,
        GRANDMOTHER_FOOTSTEP
    };
    enum class Lidar
    {
        INVALID = -1,
        STEP,
        CONTINUOUS
    };

private:
    Mode mode_;
    Lidar lidar_;

public:
    Arguments();
    Arguments(int argc, const char* argv[]);

    /**
     * Print the help content.
     *
     * Enumerate each arguments and the conditions.
     *
     * @param out The output to print to.
     */
    void printHelp(std::ostream& out);

    /**
     * Parse the arguments and determine the mode to run.
     *
     * @param argc The number of arguments.
     * @param argv The list of arguments.
     * @param mode [out] The selected mode or -1 if the selection is not done.
     */
    void parseArgs(int argc, const char* argv[]);

    inline Mode mode() const { return mode_; }
    inline Lidar lidar() const { return lidar_; }
};
#endif // __ARGUMENTS_H__
