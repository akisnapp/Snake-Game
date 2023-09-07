#include "mbed.h"
#include "Nav_Switch.h"


Nav_Switch::Nav_Switch (PinName up,PinName down,PinName left,PinName right,PinName center):
    _pins(up, down, left, right, center)
{
    _pins.mode(PullUp); //needed if pullups not on board or a bare nav switch is used - delete otherwise
    wait(0.001); //delays just a bit for pullups to pull inputs high
}


bool Nav_Switch::up()
{
    return !(_pins[0]);
}
bool Nav_Switch::down()
{
    return !(_pins[1]);
}
bool Nav_Switch::left()
{
    return !(_pins[2]);
}
bool Nav_Switch::right()
{
    return !(_pins[3]);
}
bool Nav_Switch::center()
{
    return !(_pins[4]);
}
int Nav_Switch::read()
{
    return _pins.read();
}
Nav_Switch::operator int ()
{
    return _pins.read();
}
bool Nav_Switch::operator[](int index)
{
    return _pins[index];
};