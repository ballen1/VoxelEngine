#ifndef H_ENGINE_SDL
#define H_ENGINE_SDL

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define MAX_TEXT_BUFFER 50
#define MAX_KEYS 34

enum Keys
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_W,
    K_A,
    K_S,
    K_D,
    K_F,
    K_SPACE,
    K_U,
    K_Q,
    K_E,
    K_R,
    K_T,
    K_Y,
    K_I,
    K_O,
    K_P,
    K_G,
    K_H,
    K_J,
    K_K,
    K_L,
    K_Z,
    K_X,
    K_C,
    K_V,
    K_B,
    K_N,
    K_M,
    K_TILDE,
    K_ENTER,
    K_BACKSPACE
};

typedef struct
WindowSDL
{

    SDL_Window* Window;
    SDL_GLContext GLContext;

    int ScreenWidth;
    int ScreenHeight;

} WindowSDL;

typedef struct
InputSDL
{
    int Quit;
    int LastKeysState[MAX_KEYS];
    int Keys[MAX_KEYS];
    char TextInputBuf[MAX_TEXT_BUFFER];
    int TextBufDirty;
} InputSDL;

typedef struct
VESong
{
    Mix_Music* Song;
} VESong;

typedef struct
VEAudioClip
{
    Mix_Chunk* Clip;
    int Channel;
} VEAudioClip;

void CreateWindowAndGLContext(WindowSDL* Window,
			      char* Title,
			      int Width,
			      int Height);

void InitializeInputs(InputSDL* Input);

int EngineInputToScancode(int key);

void PollForInput(InputSDL* Input);

int
WasKeyPressed(InputSDL* Input, int Key);

void
StartCapturingTextInput();

void
StopCapturingTextInput();

void
PullTextInput(InputSDL* Input, char* Text);

void
VELoadSong(VESong* Song, char* Path);

void
VELoadAudioClip(VEAudioClip* Clip, char* Path, int Channel);

int
VEIsMusicPlaying();

void
VEPlayMusic(VESong* Song, int Loops);

void
VEPauseMusic();

void
VEResumeMusic();

void
VEStopMusic();

void
VEPlayAudioClip(VEAudioClip* Clip, int Loops);

int
VEIsChannelPlaying(VEAudioClip* Clip);

void
VEStopAudioClip(VEAudioClip* Clip);

void
VEFadeAudioClip(VEAudioClip* Clip, int Milliseconds);

#endif
