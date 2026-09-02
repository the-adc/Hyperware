#ifndef ENGINE_CLOCK_H_
#define ENGINE_CLOCK_H_

#include <SDL.h>

namespace engine {

class Clock {
public:
	Clock();

	int Tick(int framerate = 0);

private:
	Uint32 last_tick_;
	Uint32 time_passed_;
	Uint32 raw_passed_;
};

}

#endif
