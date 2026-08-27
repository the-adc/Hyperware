#include "engine/Clock.h"

Clock::Clock() : last_tick(SDL_GetTicks()), timepassed(0), rawpassed(0) {}

int Clock::tick(int framerate) {
    if (framerate) {
        int endtime = (int)((1.0 / (double)framerate) * 1000.0);
        rawpassed = SDL_GetTicks() - last_tick;
        int delay = endtime - (int)rawpassed;
        if (delay < 0) {
            delay = 0;
        }
        if (delay > 0) {
            SDL_Delay((Uint32)delay);
        }
    }

    Uint32 nowtime = SDL_GetTicks();
    timepassed = nowtime - last_tick;
    if (!framerate) {
        rawpassed = timepassed;
    }
    last_tick = nowtime;

    return (int)timepassed;
}
