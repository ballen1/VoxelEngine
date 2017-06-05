#include "engine_ui.h"

#include <stdio.h>

global FontRenderConfig TextRenderConfig;
global FontTextureAtlas FontAtlas;
global GLuint FontShaderProgram;

global VUI_TextElement FPSCounter;
global VUI_TextElement CubeCounter;
global VUI_TextElement PhysicsInfo;
global VUI_TextElement PlayerInfo;
global VUI_TextElement ConsoleBox;

global WindowSDL* WindowRef;

global float SumTime = 0;
global int Wait = 300;

void
LayoutTextElements(WindowSDL* Window, int CubeTotal)
{
    WindowRef = Window;

    CreateShaderProgram(&FontShaderProgram, "../data/shaders/text_vertex.vert",
			"../data/shaders/text_fragment.frag");

    LoadCharacterMapping(&FontAtlas, 0, "../data/font/TestFont.fnt");

    InitializeFontRenderer(&TextRenderConfig,
			   &FontAtlas.AtlasImage,
			   5,
			   2500,
			   FontShaderProgram);

    // FPS COUNTER

    FPSCounter.Text = malloc(sizeof(char)*10);
    sprintf(FPSCounter.Text, "FPS: %.1f", 0.0);

    FPSCounter.RenderConfig = &TextRenderConfig;
    FPSCounter.Buffer = 0;
    FPSCounter.Position = Vec2f(15.0f, 30.0f);

    CreateTextBuffer(FPSCounter.Text, 0,
		     &TextRenderConfig,
		     &FontAtlas,
		     FPSCounter.Position,
		     12,
		     Window);
    // CUBE COUNTER
    CubeCounter.Text = malloc(sizeof(char)*50);
    sprintf(CubeCounter.Text, "Rendered Voxels: %d", CubeTotal);
    CubeCounter.RenderConfig = &TextRenderConfig;
    CubeCounter.Buffer = 1;
    CubeCounter.Position = Vec2f(15.0f, Window->ScreenHeight-10.0f);

    CreateTextBuffer(CubeCounter.Text, 1,
		     &TextRenderConfig,
		     &FontAtlas,
		     CubeCounter.Position,
		     12,
		     Window);

    // PHYSICS INFORMATION
    PhysicsInfo.Text = malloc(sizeof(char)*200);
    sprintf(PhysicsInfo.Text, "Physics Info:");
    PhysicsInfo.RenderConfig = &TextRenderConfig;
    PhysicsInfo.Buffer = 2;
    PhysicsInfo.Position = Vec2f(15.0f, Window->ScreenHeight-35.0f);

    CreateTextBuffer(PhysicsInfo.Text, 2,
		     &TextRenderConfig,
		     &FontAtlas,
		     PhysicsInfo.Position,
		     8,
		     Window);

    // PLAYER INFORMATION
    PlayerInfo.Text = malloc(sizeof(char)*200);
    sprintf(PlayerInfo.Text, "Free Fly Mode:");
    PlayerInfo.RenderConfig = &TextRenderConfig;
    PlayerInfo.Buffer = 3;
    PlayerInfo.Position = Vec2f(Window->ScreenWidth-225.0, 30.0f);

    CreateTextBuffer(PlayerInfo.Text, 3,
		     &TextRenderConfig,
		     &FontAtlas,
		     PlayerInfo.Position,
		     12,
		     Window);

    // CONSOLE
    ConsoleBox.Text = malloc(sizeof(char)*600);
    sprintf(ConsoleBox.Text, "");
    ConsoleBox.RenderConfig = &TextRenderConfig;
    ConsoleBox.Buffer = 4;
    ConsoleBox.Position = Vec2f(300, 500);

    CreateTextBuffer(ConsoleBox.Text, 4,
		     &TextRenderConfig,
		     &FontAtlas,
		     ConsoleBox.Position,
		     16,
		     Window);
}

void
UpdateTextElements(GameClock* Clock, VEPlayer* Player, VEConsole* Console,
		   int CubeTotal)
{

    SumTime += Clock->DeltaTime;

    if (SumTime > Wait)
    {

	sprintf(FPSCounter.Text, "FPS: %.1f", Clock->FPS);
	CreateTextBuffer(FPSCounter.Text, 0,
			 &TextRenderConfig,
			 &FontAtlas,
			 FPSCounter.Position,
			 12,
			 WindowRef);
	
	SumTime = 0;
    }

    sprintf(CubeCounter.Text, "Rendered Voxels: %d", CubeTotal);
    CreateTextBuffer(CubeCounter.Text, 1,
		     &TextRenderConfig,
		     &FontAtlas,
		     CubeCounter.Position,
		     12,
		     WindowRef);

    sprintf(PhysicsInfo.Text, "Physics Info: Position(%.3f, %.3f, %.3f)",
	    Player->Body.Position.x, Player->Body.Position.y, Player->Body.Position.z);

    CreateTextBuffer(PhysicsInfo.Text, 2,
		     &TextRenderConfig,
		     &FontAtlas,
		     PhysicsInfo.Position,
		     10,
		     WindowRef);

    if (Player->FlyMode)
    {
	sprintf(PlayerInfo.Text, "Free Fly Mode: On");
    }
    else
    {
	sprintf(PlayerInfo.Text, "Free Fly Mode: Off");
    }

    CreateTextBuffer(PlayerInfo.Text, 3,
		     &TextRenderConfig,
		     &FontAtlas,
		     PlayerInfo.Position,
		     12,
		     WindowRef);

    if (Console->Enabled)
    {
	sprintf(ConsoleBox.Text, ">> %s", Console->CmdBuffer);
    }
    else
    {
	sprintf(ConsoleBox.Text, "");
    }

    CreateTextBuffer(ConsoleBox.Text, 4,
		     &TextRenderConfig,
		     &FontAtlas,
		     ConsoleBox.Position,
		     16,
		     WindowRef);

}

void
RenderUI()
{
    RenderText(0, &TextRenderConfig);
    RenderText(1, &TextRenderConfig);
    RenderText(2, &TextRenderConfig);
    RenderText(3, &TextRenderConfig);
    RenderText(4, &TextRenderConfig);
}
