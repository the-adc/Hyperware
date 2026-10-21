#include "engine/Events.h"

#include <deque>

namespace {

std::deque<Event> &queue() {
    static std::deque<Event> pending;
    return pending;
}

}

namespace events {

void post(const Event &event) {
    queue().push_back(event);
}

std::vector<Event> get() {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        Event event;
        switch (sdl_event.type) {
            case SDL_QUIT:
                event.type = EVENT_QUIT;
                break;
            case SDL_KEYDOWN:
                event.type = EVENT_KEYDOWN;
                event.key = sdl_event.key.keysym.sym;
                if (sdl_event.key.repeat) {
                    continue;
                }
                break;
            case SDL_KEYUP:
                event.type = EVENT_KEYUP;
                event.key = sdl_event.key.keysym.sym;
                break;
            case SDL_MOUSEBUTTONDOWN:
                event.type = EVENT_MOUSEBUTTONDOWN;
                event.button = sdl_event.button.button;
                event.x = sdl_event.button.x;
                event.y = sdl_event.button.y;
                break;
            case SDL_MOUSEBUTTONUP:
                event.type = EVENT_MOUSEBUTTONUP;
                event.button = sdl_event.button.button;
                event.x = sdl_event.button.x;
                event.y = sdl_event.button.y;
                break;
            case SDL_MOUSEMOTION:
                event.type = EVENT_MOUSEMOTION;
                event.x = sdl_event.motion.x;
                event.y = sdl_event.motion.y;
                break;
            default:
                continue;
        }
        queue().push_back(event);
    }

    std::vector<Event> result(queue().begin(), queue().end());
    queue().clear();
    return result;
}

}
