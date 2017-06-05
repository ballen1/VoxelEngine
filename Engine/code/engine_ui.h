#ifndef H_ENGINE_UI
#define H_ENGINE_UI

#include "engine_renderer.h"
#include "engine_math.h"
#include "engine_player.h"
#include "engine_clock.h"
#include "engine_console.h"

typedef struct 
VUI_TextElement
{

    char* Text;
    FontRenderConfig* RenderConfig;
    int Buffer;
    vec2f_t Position;
} VUI_TextElement;

void
LayoutTextElements(WindowSDL* Window, int CubeTotal);

void
UpdateTextElements(GameClock* Clock, VEPlayer* Player, VEConsole* Console,
		   int CubeTotal);

void
RenderUI();

#endif
