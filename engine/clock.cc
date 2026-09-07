#include "engine/clock.h"

namespace engine {

Clock::Clock() : last_tick_(SDL_GetTicks()), time_passed_(0), raw_passed_(0) {}

int Clock::Tick(int framerate) {
	if (framerate) {
		int endtime =
			static_cast<int>((1.0 / static_cast<double>(framerate)) * 1000.0);
		raw_passed_ = SDL_GetTicks() - last_tick_;
		int delay = endtime - static_cast<int>(raw_passed_);
		if (delay < 0) {
			delay = 0;
		}
		if (delay > 0) {
			SDL_Delay(static_cast<Uint32>(delay));
		}
	}

	Uint32 nowtime = SDL_GetTicks();
	time_passed_ = nowtime - last_tick_;
	if (!framerate) {
		raw_passed_ = time_passed_;
	}
	last_tick_ = nowtime;

	return static_cast<int>(time_passed_);
}

}  // namespace engine
