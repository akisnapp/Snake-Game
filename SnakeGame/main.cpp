// PROJECT INCLUDES
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "viper.h"

#include <math.h>
#include <stdio.h>

// Important definitions


/////////////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARS
Viper viper;
int timeADJ;

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES
void playSound(char* wav);
int get_action(GameInputs inputs);
int update_game(int action);
void draw_upper_status();
void draw_lower_status();
void draw_border();
void draw_game(int draw_option);
void init_main_map();

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION DEFINITIONS
/**
 * Program entry point! This is where it all begins.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    GameInputs inputs = read_inputs(); 
    start_game();
    while(inputs.b1 != 0) {
        inputs = read_inputs();
    }
    clear_screen();
    timeADJ = 0;
    viper_init(&viper);
    
    // Initialize the maps
    maps_init();
    init_main_map();

    // Initialize game state
    set_active_map(0);
    viper.head_x = viper.head_y = 5;
    
    // Initial drawing
    draw_game(FULL_DRAW);
    
    
    
    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();

        // Read inputs 
        GameInputs inputs = read_inputs();
        
        // Determine action (move, act, menu, etc.)
        int action = get_action(inputs);
        
        // Update game
        int result = update_game(action);

        uLCD.filled_rectangle(0, 0, 160, 7, BLACK);
        char str[1024];
        snprintf(str,sizeof(str),"Pos:%d,%d Score:%d",viper.head_x,viper.head_y,viper.score);
        uLCD.text_string(str,0,0,FONT_7X8,RED);
        
        // Check for game over based on result and if so, handle game over

        
        // If game is over, then draw and display text for game over sign
                
        // Draw screen
        draw_game(result);
        
        // Compute update time
        t.stop();
        int dt = t.read_ms();
        // Display and wait
        // NOTE: Text is 8 pixels tall
        if (dt < 100) wait_ms(100 - dt);
    }
}

/**
 * Plays a .wav file
 */
void playSound(char* wav)
{
    // This eventually ended up being implemented somewhere else
    
}


int get_action(GameInputs inputs)
{
    
    // Check action and menu button inputs and return the corresponding action value
    if (inputs.b1 == 0) {
        if (viper.sticky == 0) {
            viper.sticky = 1;
        } else if (viper.sticky == 1) {
            viper.sticky = 0;
        }
        
    }
    if (inputs.b2 == 0) {
        if (timeADJ < 500) {
            timeADJ = timeADJ + 50;
        }
    }
    if (inputs.b3 == 0) {
        if (timeADJ >= 50) {
            timeADJ = timeADJ - 50;
        }
    }
    //Check navigation switch inputs and return the corresponding action value
    if (inputs.ns_up == 1) {
        return GO_UP;
    }
    if (inputs.ns_down == 1) {
        return GO_DOWN;
    }
    if (inputs.ns_left == 1) {
        return GO_LEFT;
    }
    if (inputs.ns_right == 1) {
        return GO_RIGHT;
    }
    if (inputs.ns_center == 1) {
        if (viper.invincible == 0) {
            viper.invincible = 1;
        } else if (viper.invincible == 1) {
            viper.invincible = 0;
        }
    }
    // If no button is pressed, just return no action value
    if (viper.sticky == 1 && viper.last_move == 1) {
        return  GO_RIGHT;
    } else if (viper.sticky == 1 && viper.last_move == 2) {
        return GO_DOWN;
    } else if (viper.sticky == 1 && viper.last_move == 3) {
        return GO_LEFT;
    } else if (viper.sticky == 1 && viper.last_move == 4) {
        return GO_UP;
    }
    return NO_ACTION;
}


int get_object(){
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.3; //50% duty cycle - max volume
    wait(.05);
    speaker=0.0;
     if (viper.invisible) {
        viper.turns = viper.turns - 1;
        if (viper.turns <= 0) {
            viper.invisible = 0;
            viper.turns = 0;
        }
    }
    if (viper.shield) {
        viper.shieldTime--;
        if (viper.shieldTime <= 0) {
            viper.shield = 0;
            viper.shieldTime = 0;
        }
    }
    if (viper.poison) {
        viper.poisonTime--;
        if (viper.poisonTime <= 0) {
            viper.poison = 0;
            viper.poisonTime = 0;
        }
    }
    if (viper.mag) {
        viper.mt--;
        MapItem *curr = get_north(viper.head_x, viper.head_y);
        if (curr->type == COIN) {
            if (!viper.poison) viper.score++;
            map_erase(viper.head_x, viper.head_y - 1);
        }
        curr = get_south(viper.head_x, viper.head_y);
        if (curr->type == COIN) {
            if (!viper.poison) viper.score++;
            map_erase(viper.head_x, viper.head_y + 1);
        }
        curr = get_east(viper.head_x, viper.head_y);
        if (curr->type == COIN) {
            if (!viper.poison) viper.score++;
            map_erase(viper.head_x + 1, viper.head_y);
        }
        curr = get_west(viper.head_x, viper.head_y);
        if (curr->type == COIN) {
            if (!viper.poison) viper.score++;
            map_erase(viper.head_x - 1, viper.head_y);
        }
        if (viper.mt <= 0) {
            viper.mag = 0;
            viper.mt = 0;
        }
    }
    // Get item at current head location
    MapItem* item = get_here(viper.head_x,viper.head_y);
    // Initialize a return value variable to denote the item type we got
    int toReturn = NULL;
    // Check if item is valid and is a chest
    // if so, increment the score, extend the length of the viper
    // Remove the chest
    // set the return value variable to GOT_LENGTH
    int x = viper.score;
    if (item->type != NULL && item->walkable == false && viper.invincible == 0 && !viper.shield || item->type == WALL || viper.length > VIPER_MAX_LENGTH) {
        GameInputs inputs = read_inputs();
        viper.score = x;
        while(inputs.b1 != 0) {
            inputs = read_inputs();
            end_game();
            wait_ms(500);
            clear_screen();
            draw_cry(11, 70);
            wait_ms(50);
            draw_cry(22, 70);
            wait_ms(50);
            draw_cry(33, 70);
            draw_cry(44, 70);
            wait_ms(50);
            draw_cry(55, 70);
            wait_ms(50);
            draw_cry(66, 70);
            wait_ms(50);
            draw_cry(77, 70);
            wait_ms(50);
            draw_cry(88, 70);
            wait_ms(50);
            draw_cry(99, 70);
            wait_ms(50);
            draw_cry(110,70);
        }
        map_destroy(get_active_map());
        clear_screen();
        wait_ms(100);
        main();
        // how do I make the game restart //RESOLVED
    }
    if (item->type != NULL && item->type == CHEST) {
        if (!viper.poison) viper.score++;
        viper.length++;
        map_erase(viper.head_x, viper.head_y);
        toReturn = GOT_LENGTH;
    }
    // Else, check if the item is valid a boost-up/boost-down
    // if so, set the ability for the viper based on the type of item
    //          for the right amount fo time
    // Remove the boost-up/down item
    // Set the return value variable to GOT_OBJ
    if (item->type != NULL && item->type == SHIELD) {
        viper.shield = 1;
        viper.shieldTime = 12;
    }
    if (item->type != NULL && item->type == POISON) {
        viper.poison = 1;
        viper.poisonTime = 12;
    }
    if (item->type != NULL && item->type == GHOST) {
        viper.invisible = 1;
        viper.turns = 12;
        toReturn = NULL;
    }
    if (item->type != NULL && item->type == COLOR_CHANGE) {
        if (viper.color == 'G') {
            viper.color = 'R';
        } else if (viper.color == 'R') {
            viper.color = 'B';
        } else if(viper.color == 'B') {
            viper.color = 'G';
        }
        toReturn = NULL;
    }
    
    if (item->type != NULL && item->type == COIN) {
        if (!viper.poison) viper.score++;
        toReturn == NULL;
    }
    if (item->type != NULL && item->type == MAGNET) {
        viper.mag = '1';
        viper.mt = 20;
    }
    // Check if the return value variable is GOT_LENGTH
    //  If so, then increase the snake length and update its locations as needed
    if (toReturn != NULL && toReturn == GOT_LENGTH) {
        int tempX = viper.locations[viper.length - 2].x;
        int tempY = viper.locations[viper.length - 2].y;
        for(int i = viper.length - 1; i >= 1; i--) {
            viper.locations[i] = viper.locations[i - 1];
        }
        viper.locations[0].x = viper.head_x;
        viper.locations[0].y = viper.head_y;
        add_viper_head(viper, viper.locations[0].x, viper.locations[0].y);
        for(int i = 1; i < viper.length - 1; i++) {
            add_viper_body(viper, viper.locations[i].x, viper.locations[i].y);
        }
        add_viper_tail(viper, tempX, tempY);
    }
    
    //    initialize two variables to to denote the original location for the x and y tail of the viper
    //    For each location of the viper body, move them forward by one location
    //    Update the initial viper location to the viper head x and y location
    //    call a function to add a viper head given the head x and y location
    //    call a function to add a viper body to the location after the head
    //    call a function to add a viper tail to the location of the last viper locations
    //    call a function to map erase the original location for the x and y tail
    //    return the return value variable
    if (toReturn == NULL) {
        int tempX = viper.locations[viper.length - 1].x;
        int tempY = viper.locations[viper.length - 1].y;
        for(int i = viper.length - 1; i >= 1; i--) {
            viper.locations[i] = viper.locations[i - 1];
        }
        viper.locations[0].x = viper.head_x;
        viper.locations[0].y = viper.head_y;
        add_viper_head(viper, viper.locations[0].x, viper.locations[0].y);
        for(int i = 1; i < viper.length - 1; i++) {
            add_viper_body(viper, viper.locations[i].x, viper.locations[i].y);
        }
        add_viper_tail(viper, viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);
        map_erase(tempX, tempY);
    }
    wait_ms(timeADJ);
    return toReturn;
}

/**
 * Update the game state based on the user action. 
 */
int update_game(int action)
{
    // Check the viper speed and update viper speed status
    
    // Update the previous viper head position to the current position
    viper.head_px = viper.head_x;
    viper.head_py = viper.head_y;
    // Check if action is to go up
            
    // Get item north of the current viper head
    // Update the current viper head position upwards
    // Check if the item exist and item is walkable
    // Check if the viper head does not collide with itself
    // If so, return game over
    // Otherwise, update viper head back to its original position and return no result.
    // Check if the viper gets an object (eats) by calling another function
    // Check if viper length is more than the maximum length
    // If so, return game over
    // Finally, return moved since the viper has moved
    if (action == GO_UP) {
     MapItem* item = get_north(viper.head_x,viper.head_y);
     viper.last_move = 4;
     viper.head_y = viper.head_y - 1;
     get_object();
     return MOVED;
    }
    // Else check if action is to go left, right, and down       
    if (action == GO_DOWN) {
         MapItem* item = get_south(viper.head_x,viper.head_y);
         viper.last_move = 2;
         viper.head_y = viper.head_y + 1;
         get_object();
         return MOVED;
    }
    
    if (action == GO_LEFT) {
         MapItem* item = get_west(viper.head_x,viper.head_y);
         viper.last_move = 3;
         viper.head_x = viper.head_x - 1;
         get_object();
         return MOVED;
    }
    
    if (action == GO_RIGHT) {
         MapItem* item = get_east(viper.head_x,viper.head_y);
         viper.last_move = 1;
         viper.head_x = viper.head_x + 1;
         get_object();
         return MOVED;
    }
    // If no action was given, then we simply return no result
    return NO_RESULT;
}




/**
 * Draw the upper status bar.
 */
void draw_upper_status()
{
    uLCD.line(0, 9, 127, 9, GREEN);
}

/**
 * Draw the lower status bar.
 */
void draw_lower_status()
{
    uLCD.line(0, 118, 127, 118, GREEN);
}

/**
 * Draw the border for the map.
 */
void draw_border()
{
    uLCD.filled_rectangle(  0,   9, 127,  14, WHITE);   // Top
    uLCD.filled_rectangle(  0,  13,   2, 114, WHITE);   // Left
    uLCD.filled_rectangle(  0, 114, 127, 117, WHITE);   // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE);   // Right
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int draw_option)
{
    // Draw game border first
    if(draw_option == FULL_DRAW) 
    {
        draw_border();
        int u = 58;
        int v = 56;
        draw_viper_head(u, v);
        draw_viper_body(u-11, v);
        draw_viper_tail(u-22, v);
        return;
    }

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + viper.head_x;
            int y = j + viper.head_y;

            // Compute the previous map (px, py) of this tile
            int px = i + viper.head_px;
            int py = j + viper.head_py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (draw_option || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                } else if (curr_item && curr_item->type == CLEAR) {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            } else if (draw_option) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status();
    draw_lower_status();
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and background to see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39) {
        add_chest(i % map_width(), i / map_width());
    }
    for(int i = map_width() + 3; i < map_area(); i += 172) {
        add_ghost(i % map_width(), i / map_width());
    }
    for(int i = map_width() + 3; i < map_area(); i += 151) {
        add_color_change(i % map_width(), i / map_width());
    }
    for(int i = map_width() + 3; i < map_area(); i += 100) {
        add_coin(i % map_width(), i / map_width());
    }
    for(int i = map_width() + 3; i < map_area(); i += 277) {
        add_magnet(i % map_width(), i / map_width());
    }
    for(int i = map_width() + 3; i < map_area(); i += 78) {
        add_rock(i % map_width(), i / map_width());
    }
    for(int i = map_width() + 3; i < map_area(); i += 276) {
        add_poison(i % map_width(), i / map_width());
    }
    for(int i = map_width() + 3; i < map_area(); i += 317) {
        add_shield(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");

    pc.printf("Adding walls!\r\n");
    add_wall(            0,              0, HORIZONTAL, map_width());
    add_wall(            0, map_height()-1, HORIZONTAL, map_width());
    add_wall(            0,              0,   VERTICAL, map_height());
    add_wall(map_width()-1,              0,   VERTICAL, map_height());
    pc.printf("Walls done!\r\n");
    
    add_viper_head(viper, viper.locations[0].x, viper.locations[0].y);
    add_viper_body(viper, viper.locations[1].x, viper.locations[1].y);
    add_viper_tail(viper, viper.locations[2].x, viper.locations[2].y);
    
    pc.printf("Add extra chamber\r\n");
    add_wall(30,  0,   VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39,  0,   VERTICAL, 10);
    pc.printf("Added!\r\n");
    
    print_map();
}
