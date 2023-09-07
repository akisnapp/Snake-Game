#include "speech.h"

#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP     0
#define BOTTOM  1
static void draw_speech_line(const char *line, int which);

void draw_speech_bubble()
{
    // uLCD methods you can use to draw graphics 
    
    //Link: https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/
    //***
}   

void erase_speech_bubble()
{
    // TODO: Implement
}

void draw_speech_line(const char *line, int which)
{
    // TODO: Implement
}

void speech_bubble_wait()
{
    // TODO: Implement
}

void speech(const char *line1, const char *line2)
{
    // TODO: Implement
}

void long_speech(const char *lines[], int n)
{
    // TODO: Implement
}
