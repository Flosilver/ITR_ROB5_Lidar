#include <iostream>
#include "shaft_encoder.h"

using namespace std;
int main(void)
{
    ShaftEncoder se("/tmp/encoder", 120);
    cout << "Increments: " << se.measureIncrements() << endl;
    cout << "Angle: " << se.measurePosition() << endl;
    return 0;
}
