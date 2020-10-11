#include <iostream>

#include <wiringPi.h>

using namespace std;

int main()
{
    wiringPiSetup(); // setup the library
    cout << "wiring PI seted up\n";

    return 0;
}
