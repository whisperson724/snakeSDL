#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

typedef enum Direction
{
    DIR_RIGHT,
    DIR_LEFT,
    DIR_UP,
    DIR_DOWN
} Direction;

typedef struct Point
{
    int x;
    int y;
} Point;

typedef struct Game
{
    int world_w;
    int world_h;
    int tile_size;

    Point *snake;
    size_t snake_len;
    size_t snake_cap;

    Point apple;
    Direction dir;
    bool game_over;
} Game;

bool game_init(Game *g, int world_w, int world_h, int tile_size, Point *snake_buf, size_t snake_cap);
void game_reset(Game *g);
void game_set_direction(Game *g, Direction dir);
void game_step(Game *g);
void game_destroy(Game *g);

#endif
