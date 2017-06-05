#ifndef H_ENGINE_CLOCK
#define H_ENGINE_CLOCK

#define FRAME_COUNT 10

typedef struct
GameClock
{

    float Time;
    float LastTime;
    float DeltaTime;
    unsigned int FrameTimes[FRAME_COUNT];
    int FrameCount;
    int FrameIndex;
    float FPS;
} GameClock;

void
StartClock(GameClock* C);

void
UpdateClock(GameClock* C);

#endif
