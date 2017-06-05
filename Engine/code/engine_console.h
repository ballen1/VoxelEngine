#ifndef H_ENGINE_CONSOLE
#define H_ENGINE_CONSOLE

#include "engine_world.h"
#include "engine_sdl.h"

#define MAX_COMMAND_BUFFER 500

typedef struct
VEConsole
{
    int Enabled;
    char CmdBuffer[MAX_COMMAND_BUFFER];
    CubeWorld* WorldRef;
    CubeType* CubeTypeRef;
} VEConsole;

void
InitializeVEConsole(VEConsole* Console,
		    CubeWorld* World, CubeType* CubeTypes);

void
UpdateVEConsole(VEConsole* Console, InputSDL* Input);

void
CloseVEConsole(VEConsole* Console, InputSDL* Input);

int
ParseVEConsoleCommand(VEConsole* Console);

#endif
