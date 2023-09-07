#include "globals.h"
#include "hardware.h"

// Instantiate some of the globals for hardware 

// Hardware initialization: Instantiate all the things!
uLCD_4DGL uLCD(p9,p10,p11);                 // LCD Screen (tx, rx, reset)
Serial pc(USBTX,USBRX);                     // USB Console (tx, rx)
DigitalIn button1(p21);                     // Pushbuttons (pin)
DigitalIn button2(p22);                     // ...
DigitalIn button3(p23);                     // ...
AnalogOut DACout(p18);                      // Speaker (pin)
PwmOut speaker(p26);                        // ...
wave_player waver(&DACout);                 // Wave player
Nav_Switch navs(p12,p15,p14,p16,p13);       // Nav Switch

/*
 * This initializes the hardware by calling some functions to set them up.
 */
int hardware_init()
{
    // Crank up the speed
    uLCD.baudrate(3000000);
    pc.baud(115200);
        
    //Initialize action pushbuttons
    button1.mode(PullUp); 
    button2.mode(PullUp);
    button3.mode(PullUp);
    
    return ERROR_NONE;
}

/*
 * This handles reading inputs from all possible devices - pushbuttons, nav_switch
 */
GameInputs read_inputs() 
{
    GameInputs in;
    
    // Read action buttons from in 
    // Low voltage means "pressed" because buttons are active low
    in.b1 = button1.read();
    in.b2 = button2.read();
    in.b3 = button3.read();
    // Read navigation switch buttons from in
    in.ns_up =  navs.up(); 
    in.ns_down = navs.down();
    in.ns_left = navs.left();
    in.ns_right = navs.right();
    in.ns_center = navs.center();
    wait(0.02); //For simple debouncing
    return in;
}
