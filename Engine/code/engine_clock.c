#include "engine_clock.h"

void
StartClock(GameClock* C)
{
    C->LastTime = SDL_GetTicks();
    memset(C->FrameTimes, 0, FRAME_COUNT);
    C->FrameCount = 0;
    C->FrameIndex = 0;
}

void
UpdateClock(GameClock* C)
{
    float CurrentTime = SDL_GetTicks();
    
    C->Time = CurrentTime;

    C->DeltaTime = CurrentTime - C->LastTime;
    C->LastTime = CurrentTime;

    C->FrameTimes[C->FrameIndex] = C->DeltaTime;
    if (C->FrameCount < FRAME_COUNT)
    {
	C->FrameCount++;
    }

    C->FrameIndex = (C->FrameIndex + 1) % FRAME_COUNT;

    float Sum = 0;

    for (int i = 0; i < C->FrameCount; i++)
    {
	Sum += C->FrameTimes[i];
    }

    Sum /= C->FrameCount;
    C->FPS = 1000.0 / Sum;

}
