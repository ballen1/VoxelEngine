#include "engine_sdl.h"

#include <GL/glew.h>
#include <stdio.h>

void CreateWindowAndGLContext(WindowSDL* Window,
			      char* Title,
			      int Width,
			      int Height)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    Window->ScreenWidth = Width;
    Window->ScreenHeight = Height;

    Window->Window = SDL_CreateWindow(Title,
				      SDL_WINDOWPOS_CENTERED,
				      SDL_WINDOWPOS_CENTERED,
				      Width,
				      Height,
				      SDL_WINDOW_OPENGL);

    if (Window->Window)
    {

	Window->GLContext = SDL_GL_CreateContext(Window->Window);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (Window->GLContext)
	{
	    GLenum GLEWError = glewInit();

	    if (!GLEWError)
	    {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, Width, Height);
	    }
	    else
	    {
		printf("Failed to initialize GLEW\n");
		exit(-1);
	    }

	}
	else
	{
	    printf("Failed to create OpenGL Context\n");
	    exit(-1);
	}

    }
    else
    {
	printf("Failed to create SDL window\n");
	exit(-1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
	printf("Failed to initialize SDL_mixer\n");
	exit(-1);
    }

}

void InitializeInputs(InputSDL* Input)
{
    Input->Quit = 0;
    Input->TextBufDirty = 0;
    
    for (int i = 0; i < MAX_KEYS; i++)
    {
	Input->Keys[i] = 0;
	Input->LastKeysState[i] = 0;
    }
}

int ScancodeToEngineKey(int Key)
{

    int EngineKey;

    switch (Key)
    {
	case SDL_SCANCODE_UP: EngineKey = K_UP; break;
	case SDL_SCANCODE_DOWN: EngineKey = K_DOWN; break;
        case SDL_SCANCODE_RIGHT: EngineKey = K_RIGHT; break;
	case SDL_SCANCODE_LEFT: EngineKey = K_LEFT; break;
	case SDL_SCANCODE_W: EngineKey = K_W; break;
        case SDL_SCANCODE_S: EngineKey = K_S; break;
	case SDL_SCANCODE_A: EngineKey = K_A; break;
        case SDL_SCANCODE_D: EngineKey = K_D; break;
	case SDL_SCANCODE_F: EngineKey = K_F; break;
	case SDL_SCANCODE_SPACE: EngineKey = K_SPACE; break;
	case SDL_SCANCODE_U: EngineKey = K_U; break;
	case SDL_SCANCODE_Q: EngineKey = K_Q; break;
	case SDL_SCANCODE_E: EngineKey = K_E; break;
	case SDL_SCANCODE_R: EngineKey = K_R; break;
	case SDL_SCANCODE_T: EngineKey = K_T; break;
	case SDL_SCANCODE_Y: EngineKey = K_Y; break;
	case SDL_SCANCODE_I: EngineKey = K_I; break;
	case SDL_SCANCODE_O: EngineKey = K_O; break;
	case SDL_SCANCODE_P: EngineKey = K_P; break;
	case SDL_SCANCODE_G: EngineKey = K_G; break;
	case SDL_SCANCODE_H: EngineKey = K_H; break;
	case SDL_SCANCODE_J: EngineKey = K_J; break;
	case SDL_SCANCODE_K: EngineKey = K_K; break;
	case SDL_SCANCODE_L: EngineKey = K_L; break;
	case SDL_SCANCODE_Z: EngineKey = K_Z; break;
	case SDL_SCANCODE_X: EngineKey = K_X; break;
	case SDL_SCANCODE_C: EngineKey = K_C; break;
	case SDL_SCANCODE_V: EngineKey = K_V; break;
	case SDL_SCANCODE_B: EngineKey = K_B; break;
	case SDL_SCANCODE_N: EngineKey = K_N; break;
	case SDL_SCANCODE_M: EngineKey = K_M; break;
	case SDL_SCANCODE_GRAVE: EngineKey = K_TILDE; break;
	case SDL_SCANCODE_RETURN: EngineKey = K_ENTER; break;
	case SDL_SCANCODE_BACKSPACE: EngineKey = K_BACKSPACE; break;
        default: EngineKey = -1; break;
    }

    return EngineKey;

}

void PollForInput(InputSDL* Input)
{

    for (int i = 0; i < MAX_KEYS; i++)
    {
	Input->LastKeysState[i] = Input->Keys[i];
    }
    
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
	switch(Event.type)
	{
	    case SDL_QUIT: {
		Input->Quit = 1;
	    } break;
	    case SDL_KEYDOWN: {

		int KeyCode = ScancodeToEngineKey(Event.key.keysym.scancode);

		if (KeyCode != -1)
		{
		    Input->Keys[KeyCode] = 1;
		}

	    } break;
	    case SDL_KEYUP: {

		int KeyCode = ScancodeToEngineKey(Event.key.keysym.scancode);

		if (KeyCode != -1)
		{
		    Input->Keys[KeyCode] = 0;
		}

	    } break;
	    case SDL_TEXTINPUT: {
		strcpy(Input->TextInputBuf, Event.text.text);
		Input->TextBufDirty = 1;
	    } break;
	    default: {

	    } break;
	}
    }
}

int
WasKeyPressed(InputSDL* Input, int Key)
{
    if (!Input->LastKeysState[Key] &&
	Input->Keys[Key])
    {
	return (1);
    }

    return (0);
}

void
StartCapturingTextInput()
{
    SDL_StartTextInput();
}

void
StopCapturingTextInput()
{
    SDL_StopTextInput();
}

void
PullTextInput(InputSDL* Input, char* Text)
{
    if (!Text)
    {
	Input->TextBufDirty = 0;
    }

    if (Input->TextBufDirty)
    {
	strcpy(Text, Input->TextInputBuf);
	Input->TextBufDirty = 0;
    }
}

void
VELoadSong(VESong* Song, char* Path)
{
    Song->Song = Mix_LoadMUS(Path);
    if (!Song->Song)
    {
	printf("Song failed to load: %s\n", Path);
	exit(-1);
    }
}

void
VELoadAudioClip(VEAudioClip* Clip, char* Path, int Channel)
{
    Clip->Clip = Mix_LoadWAV(Path);
    if (!Clip->Clip)
    {
	printf("Audio clip failed to load: %s\n", Path);
	exit(-1);
    }

    Clip->Channel = Channel;
}

int
VEIsMusicPlaying()
{
    return Mix_PlayingMusic();
}

void
VEPlayMusic(VESong* Song, int Loops)
{
    if (Mix_PlayMusic(Song->Song, Loops) < 0)
    {
	printf("Catastrophic error when attempting to play music.\n");
	printf("SDL_mixer error: %s\n", Mix_GetError());
	exit(-1);
    }
}

void
VEPauseMusic()
{
    Mix_PauseMusic();
}

void
VEResumeMusic()
{
    Mix_ResumeMusic();
}

void
VEStopMusic()
{
    Mix_HaltMusic();
}

void
VEPlayAudioClip(VEAudioClip* Clip, int Loops)
{
    Mix_PlayChannel(Clip->Channel, Clip->Clip, Loops);
}

int
VEIsChannelPlaying(VEAudioClip* Clip)
{
    return (Mix_Playing(Clip->Channel));
}

void
VEStopAudioClip(VEAudioClip* Clip)
{
    Mix_HaltChannel(Clip->Channel);
}

void
VEFadeAudioClip(VEAudioClip* Clip, int Milliseconds)
{
    Mix_FadeOutChannel(Clip->Channel, Milliseconds);
}
