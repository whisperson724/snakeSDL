#include "game.h"
#include <stdlib.h>

static bool points_equal(Point a, Point b)
{
    return a.x == b.x && a.y == b.y;
}

static bool is_opposite(Direction a, Direction b)
{
    return (a == DIR_LEFT  && b == DIR_RIGHT) ||
           (a == DIR_RIGHT && b == DIR_LEFT)  ||
           (a == DIR_UP    && b == DIR_DOWN)  ||
           (a == DIR_DOWN  && b == DIR_UP);
}

static bool snake_contains(const Game *g, Point p)
{
    for (size_t i = 0; i < g->snake_len; ++i)
    {
        if (points_equal(g->snake[i], p)) 
        {
            return true;
        }
    }
    return false;
}

static void spawn_apple(Game *g)
{
    int cols = g->world_w / g->tile_size;
    int rows = g->world_h / g->tile_size;

    Point p;
    do
    {
        p.x = (rand() % cols) * g->tile_size;
        p.y = (rand() % rows) * g->tile_size;
    } while (snake_contains(g, p));

    g->apple = p;
}

static void grow_snake(Game *g)
{
    if (g->snake_len < g->snake_cap)
    {
        g->snake[g->snake_len] = g->snake[g->snake_len - 1];
        g->snake_len++;
    }
}

static void wrap_head(Game *g)
{
    if (g->snake[0].x < 0) g->snake[0].x = g->world_w - g->tile_size;
    else if (g->snake[0].x >= g->world_w) g->snake[0].x = 0;

    if (g->snake[0].y < 0) g->snake[0].y = g->world_h - g->tile_size;
    else if (g->snake[0].y >= g->world_h) g->snake[0].y = 0;
}

static bool snake_hit_itself(const Game *g)
{
    for (size_t i = 1; i < g->snake_len; ++i)
    {
        if (points_equal(g->snake[0], g->snake[i]))
        {
            return true;
        }
    }
    return false;
}

bool game_init(Game *g, int world_w, int world_h,
        int tile_size, Point *snake_buf, size_t snake_cap)
{
    if (!g || !snake_buf || snake_cap == 0 || tile_size <= 0)
    {
        return false;
    }

    g->world_w = world_w;
    g->world_h = world_h;
    g->tile_size = tile_size;
    g->snake = snake_buf;
    g->snake_cap = snake_cap;

    game_reset(g);
    return true;
}

void game_reset(Game *g)
{
    g->snake_len = 1;
    g->snake[0].x = 0;
    g->snake[0].y = 0;
    g->dir = DIR_RIGHT;
    g->game_over = false;
    spawn_apple(g);
}

void game_set_direction(Game *g, Direction dir)
{
    if (!is_opposite(g->dir, dir) || g->snake_len == 1)
    {
        g->dir = dir;
    }
}

void game_step(Game *g)
{
    for (size_t i = g->snake_len - 1; i > 0; --i)
    {
        g->snake[i] = g->snake[i - 1];
    }

    switch (g->dir)
    {
        case DIR_RIGHT: g->snake[0].x += g->tile_size; break;
        case DIR_LEFT:  g->snake[0].x -= g->tile_size; break;
        case DIR_UP:    g->snake[0].y -= g->tile_size; break;
        case DIR_DOWN:  g->snake[0].y += g->tile_size; break;
    }

    wrap_head(g);

    if (snake_hit_itself(g))
    {
        game_reset(g);
        return;
    }

    if (points_equal(g->snake[0], g->apple))
    {
        grow_snake(g);
        spawn_apple(g);
    }
}

void game_destroy(Game *g)
{
    (void)g;
}
