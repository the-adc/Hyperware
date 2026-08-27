#ifndef ENGINE_CLOCK_H
#define ENGINE_CLOCK_H

#include <SDL.h>

class Clock {
public:
    Clock();

    int tick(int framerate = 0);

private:
    Uint32 last_tick;
    Uint32 timepassed;
    Uint32 rawpassed;
};

#endif
