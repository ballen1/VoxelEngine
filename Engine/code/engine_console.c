#include "engine_console.h"

#include <stdio.h>

void
InitializeVEConsole(VEConsole* Console,
		    CubeWorld* World, CubeType* CubeTypes)
{
    Console->Enabled = 0;
    Console->WorldRef = World;
    Console->CubeTypeRef = CubeTypes;
    memset(Console->CmdBuffer, 0, sizeof(Console->CmdBuffer));
    StopCapturingTextInput();
}

void
UpdateVEConsole(VEConsole* Console, InputSDL* Input)
{
    if (WasKeyPressed(Input, K_TILDE))
    {
	Console->Enabled = (!Console->Enabled);
	if (Console->Enabled)
	{
	    StartCapturingTextInput();
	}
	else
	{
	    CloseVEConsole(Console, Input);
	}
    }
    if (Console->Enabled)
    {
	if (WasKeyPressed(Input, K_ENTER))
	{
	    int Result = ParseVEConsoleCommand(Console);
	    memset(Console->CmdBuffer, '\0', sizeof(Console->CmdBuffer));
	    if (!Result)
	    {
		sprintf(Console->CmdBuffer, "Error with command");
	    }
	    if (Result)
	    {
		CloseVEConsole(Console, Input);
	    }
	}
	else if (WasKeyPressed(Input, K_BACKSPACE))
	{
	    int BIndex = 0;
	    while (BIndex < MAX_COMMAND_BUFFER && Console->CmdBuffer[BIndex] != '\0')
	    {
		BIndex++;
	    }
	    if (BIndex != 0)
	    {
		Console->CmdBuffer[BIndex-1] = '\0';
	    }
	}
	else
	{
	    char Text[50];
	    memset(Text, '\0', sizeof(Text));
	    PullTextInput(Input, Text);
	    sprintf(Console->CmdBuffer, "%s%s", Console->CmdBuffer, Text);
	}
    }
}

void
CloseVEConsole(VEConsole* Console, InputSDL* Input)
{
    Console->Enabled = 0;
    StopCapturingTextInput();
    PullTextInput(Input, 0);
    memset(Console->CmdBuffer, '\0', sizeof(Console->CmdBuffer));    
}

int
ParseVEConsoleCommand(VEConsole* Console)
{
    int Success = 0;
    int Index = 0;
    int BufIndex = 0;

    int CmdMode = 0;
    int ArgIndex = 0;
    float Args[4];

    int RebuildArgs[3];

    char Buf[100];
    memset(Buf, '\0', sizeof(Buf)); 

    while (Index < MAX_COMMAND_BUFFER && Console->CmdBuffer[Index] != '\0')
    {
	if (Console->CmdBuffer[Index] != '\0')
	{
	    if (CmdMode == 1)
	    {
		if (Console->CmdBuffer[Index] != ',')
		{
		    Buf[BufIndex] = Console->CmdBuffer[Index];
		    BufIndex++;
		}
		if (Console->CmdBuffer[Index] == ',' || Console->CmdBuffer[Index] == ';')
		{
		    Args[ArgIndex] = atof(Buf);
		    ArgIndex++;
		    BufIndex = 0;
		    memset(Buf, '\0', sizeof(Buf));
		}
		if (ArgIndex == 4)
		{
		    Success = 1;
		    ReseedWorld(Console->WorldRef, Console->CubeTypeRef,
				Args[0], Args[1], 0.01f, Args[2], Args[3]);
		}
	    }
	    else if (CmdMode == 2)
	    {
		VEResumeMusic();
		Success = 1;
	    }
	    else if (CmdMode == 3)
	    {
		VEPauseMusic();
		Success = 1;
	    }
	    else if (CmdMode == 4)
	    {
		Console->WorldRef->DayNightCycle = 1;
		Success = 1;
	    }
	    else if (CmdMode == 5)
	    {
		Console->WorldRef->DayNightCycle = 0;
		Success = 1;
	    }
	    else if (CmdMode == 6)
	    {
		Console->WorldRef->Apocalypse = 1;
		Success = 1;
	    }
	    else if (CmdMode == 7)
	    {
		Console->WorldRef->Apocalypse = 0;
		Success = 1;
	    }
	    else if (CmdMode == 8)
	    {
		Console->WorldRef->Spheres = 1;
		Success = 1;
	    }
	    else if (CmdMode == 9)
	    {
		Console->WorldRef->Spheres = 0;
		Success = 1;
	    }
	    else if (CmdMode == 10)
	    {
		if (Console->CmdBuffer[Index] != ',')
		{
		    Buf[BufIndex] = Console->CmdBuffer[Index];
		    BufIndex++;
		}
		if (Console->CmdBuffer[Index] == ',' || Console->CmdBuffer[Index] == ';')
		{
		    RebuildArgs[ArgIndex] = atoi(Buf);
		    ArgIndex++;
		    BufIndex = 0;
		    memset(Buf, '\0', sizeof(Buf));
		}
		if (ArgIndex == 3)
		{
		    Success = 1;
		    RedoWorld(Console->WorldRef,
			      RebuildArgs[0],
			      RebuildArgs[1],
			      RebuildArgs[2],
			      32,
			      1.0, 1.0, 0.01, 6, 0.5f);
		}
	    }
	    else
	    {
		if (Console->CmdBuffer[Index] != ':')
		{
		    Buf[Index] = Console->CmdBuffer[Index];
		}
		if (Console->CmdBuffer[Index] == ':')
		{
		    if (!strcmp(Buf, "hatch"))
		    {
			CmdMode = 1;
		    }
		    if (!strcmp(Buf, "voxel_dj"))
		    {
			CmdMode = 2;
		    }
		    if (!strcmp(Buf, "sound_of_silence"))
		    {
			CmdMode = 3;
		    }
		    if (!strcmp(Buf, "sands_of_time"))
		    {
			CmdMode = 4;
		    }
		    if (!strcmp(Buf, "time_goes_so_fast"))
		    {
			CmdMode = 5;
		    }
		    if (!strcmp(Buf, "start_the_apocalypse"))
		    {
			CmdMode = 6;
		    }
		    if (!strcmp(Buf, "have_mercy"))
		    {
			CmdMode = 7;
		    }
		    if (!strcmp(Buf, "show_sphere"))
		    {
			CmdMode = 8;
		    }
		    if (!strcmp(Buf, "unshow_sphere"))
		    {
			CmdMode = 9;
		    }
		    if (!strcmp(Buf, "we_can_rebuild_it"))
		    {
			CmdMode = 10;
		    }
		    memset(Buf, '\0', sizeof(Buf));
		}
	    }
	}
	Index++;
    }

    return (Success);
}
