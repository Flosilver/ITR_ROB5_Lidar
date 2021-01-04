#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include <iostream>

enum Mode
{
    INVALID = -1,
    PROXIMITY_TRACKER,
    GRANDMOTHER_FOOTSTEP
};

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
void parseArgs(int argc, const char* argv[], Mode& mode);

#endif // __ARGUMENTS_H__
