#include "engine/events.h"

#include <deque>

namespace engine {

namespace {

std::deque<Event>& Queue() {
	static auto* pending = new std::deque<Event>();
	return *pending;
}

}

namespace events {

void Post(const Event& event) { Queue().push_back(event); }

std::vector<Event> Get() {
	SDL_Event sdl_event;
	while (SDL_PollEvent(&sdl_event)) {
		Event event;
		switch (sdl_event.type) {
			case SDL_QUIT:
				event.type = EventType::kQuit;
				break;
			case SDL_KEYDOWN:
				event.type = EventType::kKeyDown;
				event.key = sdl_event.key.keysym.sym;
				if (sdl_event.key.repeat) {
					continue;
				}
				break;
			case SDL_KEYUP:
				event.type = EventType::kKeyUp;
				event.key = sdl_event.key.keysym.sym;
				break;
			case SDL_MOUSEBUTTONDOWN:
				event.type = EventType::kMouseButtonDown;
				event.button = sdl_event.button.button;
				event.x = sdl_event.button.x;
				event.y = sdl_event.button.y;
				break;
			case SDL_MOUSEBUTTONUP:
				event.type = EventType::kMouseButtonUp;
				event.button = sdl_event.button.button;
				event.x = sdl_event.button.x;
				event.y = sdl_event.button.y;
				break;
			case SDL_MOUSEMOTION:
				event.type = EventType::kMouseMotion;
				event.x = sdl_event.motion.x;
				event.y = sdl_event.motion.y;
				break;
			default:
				continue;
		}
		Queue().push_back(event);
	}

	std::vector<Event> result(Queue().begin(), Queue().end());
	Queue().clear();
	return result;
}

}

}
