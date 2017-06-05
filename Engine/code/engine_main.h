#ifndef H_ENGINE_MAIN
#define H_ENGINE_MAIN

#define internal static
#define global static

enum GameMode
{
    SHOULD_EXIT,
    RUNNING
};

typedef struct
GameState
{
    enum GameMode mode;
} GameState;

#endif
