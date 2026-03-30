#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include "game.h"

#define TILE_SIZE 20
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_SNAKE_LEN ((WINDOW_WIDTH / TILE_SIZE) * (WINDOW_HEIGHT / TILE_SIZE))

typedef struct App
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Game game;
    Point snake_storage[MAX_SNAKE_LEN];
} App;

static void handle_input(App *app)
{
    int numkeys = 0;
    const bool *keys = SDL_GetKeyboardState(&numkeys);

    if (keys[SDL_SCANCODE_W]) game_set_direction(&app->game, DIR_UP);
    else if (keys[SDL_SCANCODE_S]) game_set_direction(&app->game, DIR_DOWN);
    else if (keys[SDL_SCANCODE_A]) game_set_direction(&app->game, DIR_LEFT);
    else if (keys[SDL_SCANCODE_D]) game_set_direction(&app->game, DIR_RIGHT);
}

static void draw_game(SDL_Renderer *renderer, const Game *g)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_FRect apple = { (float)g->apple.x, (float)g->apple.y, 
        (float)g->tile_size, (float)g->tile_size };

    SDL_SetRenderDrawColor(renderer, 220, 30, 30, 255);
    SDL_RenderFillRect(renderer, &apple);

    SDL_SetRenderDrawColor(renderer, 25, 106, 11, 255);
    for (size_t i = 0; i < g->snake_len; ++i)
    {
        SDL_FRect r = {
            (float)g->snake[i].x,
            (float)g->snake[i].y,
            (float)g->tile_size,
            (float)g->tile_size
        };
        SDL_RenderFillRect(renderer, &r);
    }

    SDL_RenderPresent(renderer);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    static App app;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Hello Daniel!", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &app.window, &app.renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!game_init(&app.game, WINDOW_WIDTH, WINDOW_HEIGHT, TILE_SIZE, app.snake_storage, MAX_SNAKE_LEN))
    {
        SDL_Log("game_init failed");
        return SDL_APP_FAILURE;
    }

    *appstate = &app;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    (void)appstate;

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    App *app = appstate;

    handle_input(app);
    game_step(&app->game);
    draw_game(app->renderer, &app->game);
    SDL_Delay(90);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void)result;
    App *app = appstate;
    if (app)
    {
        game_destroy(&app->game);
    }
}
