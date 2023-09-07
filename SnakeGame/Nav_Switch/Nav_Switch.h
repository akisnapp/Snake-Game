#include "mbed.h"


class Nav_Switch
{
public:
    Nav_Switch(PinName up,PinName down,PinName left,PinName right,PinName fire);
    int read();
//boolean functions to test each switch
    bool up();
    bool down();
    bool left();
    bool right();
    bool center();
//automatic read on RHS
    operator int ();
//index to any switch array style
    bool operator[](int index);

private:
    BusIn _pins;

};