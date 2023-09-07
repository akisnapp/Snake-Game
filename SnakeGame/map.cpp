#include "map.h"

#include "globals.h"
#include "graphics.h"

#include "viper.h"
#define MHF_NBUCKETS 97

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

#define NUM_MAPS 1

static Map maps[NUM_MAPS];
static int active_map;

static const MapItem CLEAR_SENTINEL = {
    .type = CLEAR,
    .draw = draw_nothing
};

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 */
static unsigned XY_KEY(int X, int Y) {
    // Return the 1-D xy key
    Map curr = maps[active_map];
    int height = curr.h;
    return X + Y * height; //Sets the key to X plus Y times the maps height to get a unique value
}

unsigned map_hash(unsigned key)
{
    // Use MHF_NBUCKETS as part of hash function in order to return the hashed key
    return key%MHF_NBUCKETS; //Simple hash function similar to the one in hash_table
}

/**
 * Initializes the map, using a hash_table, setting the width and height.
 */
void maps_init()
{
    // Loop through all map items and and initialize hash table to hold the buckets
    // Set width & height for any maps 
    // Set the first map to be active
    for (int i = 0; i < NUM_MAPS; i++) {
        maps[i].items = createHashTable(map_hash, MHF_NBUCKETS);
        maps[i].w = 70;//? WHAT SHOULD I SET THIS EQUAL TO, SHOULD THIS BE A CONSTANT LIKE 60 OR A VARIABLE? //RESOLVED
        maps[i].h = 70;//? WHAT SHOULD I SET THIS EQUAL TO //RESOLVED
    }
    set_active_map(0);
}

Map* get_active_map()
{
    return &maps[active_map];
}

Map* set_active_map(int m)
{
    active_map = m;
    return &maps[active_map];
}

void print_map()
{
    char lookup[] = {'W', 'D', 'P', 'A', 'K', 'C', 'N',' ','S'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

/**
 * Returns width of active map
 */
int map_width()
{
    Map* curr = get_active_map();
    return curr->w;
}

/**
 * Returns height of active map
 */
int map_height()
{
    Map* curr = get_active_map();
    return curr->h;
}

/**
 * Returns the area of the active map
 */
int map_area()
{
    return map_width() * map_height();
}

/**
 * Returns MapItem at current coordinate location
 */
MapItem* get_current(int x, int y)
{ 
    // Get map item
    // Check and remove item if item exist and is clear type
    // Return the item
    Map *curr = get_active_map();
    int key = XY_KEY(x,y); // Finds key at the current location
    // HOW DO I REMOVE ITEM IF IT EXISTS AND IS CLEAR TYPE? LOOK AT #2
    MapItem* item = (MapItem*)getItem(curr->items, key);
    if (item != NULL && item->type == CLEAR) {
        return (MapItem*)removeItem(curr->items,key);
    }
    return item;
}

/**
 * Returns the MapItem immediately above the given location.
 */
MapItem* get_north(int x, int y)
{
    // Get map item 
    // Check and remove item if item exist and is clear type
    // Return the item 
    Map *curr = get_active_map();
    int key = XY_KEY(x,y-1); //Up is negative so y-1 is one north
    
    MapItem* item = (MapItem*)getItem(curr->items, key);
    if (item != NULL && item->type == CLEAR) {
        return (MapItem*)removeItem(curr->items,key);
    }
    return item;
}

/**
 * Returns the MapItem immediately below the given location.
 */
MapItem* get_south(int x, int y)
{
    // Get map item
    // Check and remove item if item exist and is clear type
    // Return the item
    Map *curr = get_active_map();
    int key = XY_KEY(x,y+1); 
    MapItem* item = (MapItem*)getItem(curr->items, key);
    if (item != NULL && item->type == CLEAR) {
        return (MapItem*)removeItem(curr->items,key);
    }
    return item;
}

/**
 * Returns the MapItem immediately right the given location.
 */
MapItem* get_east(int x, int y)
{
    // Get map item
    // Check and remove item if item exist and is clear type
    // Return the item
    Map *curr = get_active_map();
    int key = XY_KEY(x+1,y);
    MapItem* item = (MapItem*)getItem(curr->items, key);
    if (item != NULL && item->type == CLEAR) {
        return (MapItem*)removeItem(curr->items,key);
    }
    return item;
}

/**
 * Returns the MapItem immediately left the given location.
 */
MapItem* get_west(int x, int y)
{
    // Get map item
    // Check and remove item if item exist and is clear type
    // Return the item
    Map *curr = get_active_map();
    int key = XY_KEY(x-1,y);
    MapItem* item = (MapItem*)getItem(curr->items, key);
    if (item != NULL && item->type == CLEAR) {
        return (MapItem*)removeItem(curr->items,key);
    }
    return item;
}

/**
 * Returns the MapItem at current coordinate location
 */
 MapItem* get_here(int x, int y)
 {
     // Get map item
     // Check and remove item if item exist and is clear type
     // Return the item
     Map *curr = get_active_map();
     int key = XY_KEY(x,y); // Finds key at the current location
     
     MapItem* item = (MapItem*)getItem(curr->items, key);
     if (item != NULL && item->type == CLEAR) {
         return (MapItem*)removeItem(curr->items,key);
     }
     return item;
 }

/**
 * Erase current location map item
 */
void map_erase(int x, int y)
{

    // Insert clear value to the map item at current location
    // Free current location map item if it exists
    Map *curr = get_active_map();
    int key = XY_KEY(x,y); // Finds key at the current location
    //MapItem newItem = CLEAR_SENTINEL;
    //newItem->type = CLEAR;
    //MapItem* item = (MapItem*)insertItem(curr->items, key, newItem);
    //MapItem* item = (MapItem*)insertItem(curr->items, key, CLEAR_SENTINEL);
    deleteItem(curr->items, key);
}
void map_destroy(Map* map) {
    destroyHashTable(map->items);
}
void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_chest(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = CHEST;
    w1->draw = draw_chest;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_shield(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SHIELD;
    w1->draw = draw_shield;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_magnet(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = MAGNET;
    w1->draw = draw_magnet;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_coin(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = COIN;
    w1->draw = draw_coin;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_color_change(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = COLOR_CHANGE;
    w1->draw = draw_color_change;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void remove_chest(int x, int y) // I'm lazy so overwrite it with a plant
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_ghost(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = GHOST;
    w1->draw = draw_ghost; // Change this to draw_ghost when you get the graphics done
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_poison(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = POISON;
    w1->draw = draw_poison;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_rock(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ROCK;
    w1->draw = draw_rock; // Change this to draw_ghost when you get the graphics done
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_viper_body(Viper v, int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIPER_BODY;
    if (v.invisible == 1) {
        w1->draw = draw_invisible;
    } else {
        if (v.color == 'R') {
            w1->draw = draw_viper_body_red;
        } else if(v.color == 'B') {
            w1->draw = draw_viper_body_blue;
        } else {
            w1->draw = draw_viper_body;
        }
    }
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_viper_head(Viper v, int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIPER_BODY;
    if (v.invisible == 1) {
        w1->draw = draw_invisible;
    } else if (v.last_move == 1) {
        if (v.color == 'R') {
            w1->draw = draw_viper_head_red;
        } else if(v.color == 'B') {
            w1->draw = draw_viper_head_blue;
        } else {
            w1->draw = draw_viper_head;
        }
    }else if (v.last_move == 2) {
        if (v.color == 'R') {
            w1->draw = draw_viper_head_down_red;
        } else if(v.color == 'B') {
            w1->draw = draw_viper_head_down_blue;
        } else {
            w1->draw = draw_viper_head_down;
        }
    } else if (v.last_move == 3) {
        if (v.color == 'R') {
            w1->draw = draw_viper_head_left_red;
        } else if(v.color == 'B') {
            w1->draw = draw_viper_head_left_blue;
        } else {
            w1->draw = draw_viper_head_left;
        }
    } else {
        if (v.color == 'R') {
            w1->draw = draw_viper_head_up_red;
        } else if(v.color == 'B') {
            w1->draw = draw_viper_head_up_blue;
        } else {
            w1->draw = draw_viper_head_up;
        }
    }
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_viper_tail(Viper v, int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIPER_BODY;
     if (v.invisible == 1) {
        w1->draw = draw_invisible;
    }else if (v.last_move == 1) {
        if (v.color == 'R') {
            w1->draw = draw_viper_tail_red;
        } else if(v.color == 'B') {
            w1->draw = draw_viper_tail_blue;
        } else {
            w1->draw = draw_viper_tail;
        }
    }else if (v.last_move == 2) {
        if (v.color == 'R') {
            w1->draw = draw_viper_tail_down_red;
        } else if(v.color == 'B') {
            w1->draw = draw_viper_tail_down_blue;
        } else {
            w1->draw = draw_viper_tail_down;
        }
    } else if (v.last_move == 3) {
        if (v.color == 'R') {
            w1->draw = draw_viper_tail_left_red;
        } else if(v.color == 'B') {
            w1->draw = draw_viper_tail_left_blue;
        } else {
            w1->draw = draw_viper_tail_left;
        }
    } else {
        if (v.color == 'R') {
            w1->draw = draw_viper_tail_up_red;
        } else if(v.color == 'B') {
            w1->draw = draw_viper_tail_up_blue;
        } else {
            w1->draw = draw_viper_tail_up;
        }
    }
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
