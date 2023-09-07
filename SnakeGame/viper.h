#ifndef VIPER_H
#define VIPER_H

#define VIPER_MAX_LENGTH 50

// Structure of coordinates in the map
typedef struct{
    int x;
    int y;
} Coordinate;

// Viper Structure
typedef struct {
    int head_x, head_y, head_px, head_py; // Location of the head of the viper
    int length; // length of the viper
    Coordinate locations[VIPER_MAX_LENGTH]; // Viper body locations
    int score; //Current score of the viper
    int last_move; // 1 = right, 2 = down, 3 = left, 4 = up
    int sticky; // if 1 then sticky buttons is on if 0 it is not on
    int invincible; // is 1 if viper is invincible is 0 otherwise
    int invisible; // is 1 if viper is invisible, is 0 otherwise
    int turns; // represents how many turns the viper must stay in power-up / power-down state
    char color; // character that represents the color of the viper.
    int mag; // Is 1 if magnet is active, is 0 otherwise
    int mt; // is the turn counter for the magnet being active
    int shield;
    int shieldTime;
    int poison;
    int poisonTime;
} Viper;

// Initialize a viper structure
// You want to assign initial values to each of the variables defined above
// in the viper structure.
void viper_init (Viper* viper);


#endif VIPER_H  // VIPER_H
