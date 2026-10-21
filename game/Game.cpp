#include "game/Game.h"

#include <vector>

#include "engine/Events.h"
#include "engine/Utils.h"

Game::Game() {
    running = true;

    SCREEN_SIZE = Vec2(1200, 800);
    SCREEN_CENTER = SCREEN_SIZE.floordiv(2);

    graphics::init(graphics::trunc(SCREEN_SIZE.x), graphics::trunc(SCREEN_SIZE.y),
                   "Game Title");
    mixer_init();

    screen = &graphics::screen();
}

Game::~Game() {
    current_scene.reset();
    previous_scene.reset();
    mixer_quit();
    graphics::quit();
}

void Game::draw() {
    screen->fill(Color(202, 228, 241));

    current_scene->draw(*screen);
}

void Game::update() {
    clock.tick(60);

    current_scene->update();
}

void Game::events() {
    std::vector<Event> queued = events::get();

    for (std::size_t i = 0; i < queued.size(); ++i) {
        const Event &event = queued[i];

        if (event.type == EVENT_QUIT) {
            quit();
        }

        if (event.type == EVENT_KEYDOWN) {
            if (event.key == SDLK_ESCAPE) {
                quit();
            }
            if (event.key == SDLK_F1) {
                toggle_music();
            }
        }

        current_scene->events(event);
    }

    return;
}

void Game::run() {
    while (running) {
        clock.tick(120);

        events();

        update();

        draw();

        graphics::update();

        previous_scene.reset();
    }
}

void Game::change_scene(std::unique_ptr<Scene> scene) {
    previous_scene = std::move(current_scene);
    current_scene = std::move(scene);
}

void Game::quit() {
    running = false;
}
