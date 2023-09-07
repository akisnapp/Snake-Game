#include "viper.h"

void viper_init (Viper * v)
{

    // Set starting coordinates for viper head and previous
     v->head_x = v->head_y = v->head_px = v->head_py = 5;
    // Initialize all locations for maximum viper body (loop through the viper)
    for (int i = 0; i< VIPER_MAX_LENGTH; i++) {
        v->locations[i].x = 0;
        v->locations[i].y = 0;
    }
    // Set starting length of your viper
    v->length = 3;
    v->last_move = 1;
    v->sticky = 0;
    v->invincible = 0;
    v->invisible = 0;
    v->turns = 0;
    v->color = 'G';
    v->mag = 0;
    v->poison = 0;
    // Set starting coordinates for the rest of viper body from starting length
    v->locations[0].x = v->head_x;
    v->locations[0].y = v->head_y;
    v->locations[1].x = v->head_x - 1;
    v->locations[1].y = v->head_y;
    v->locations[2].x = v->head_x - 2;
    v->locations[2].y = v->head_y;
    // Initialize viper status
    v->score = 0;
    
}
