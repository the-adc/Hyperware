#include "game/game.h"

#include <cstddef>
#include <utility>
#include <vector>

#include "engine/events.h"
#include "engine/utils.h"

namespace game {

using engine::Color;
using engine::Event;
using engine::EventType;

Game::Game() {
	running_ = true;

	screen_size_ = engine::Vec2(1200, 800);
	screen_center_ = screen_size_.FloorDiv(2);

	engine::graphics::Init(engine::graphics::Trunc(screen_size_.x),
		engine::graphics::Trunc(screen_size_.y), "Game Title");
	engine::MixerInit();

	screen_ = &engine::graphics::Screen();
}

Game::~Game() {
	current_scene_.reset();
	previous_scene_.reset();
	engine::MixerQuit();
	engine::graphics::Quit();
}

void Game::Draw() {
	screen_->Fill(Color(202, 228, 241));

	current_scene_->draw(*screen_);
}

void Game::Update() {
	clock_.Tick(60);

	current_scene_->update();
}

void Game::Events() {
	std::vector<Event> queued = engine::events::Get();

	for (size_t i = 0; i < queued.size(); ++i) {
		const Event& event = queued[i];

		if (event.type == EventType::kQuit) {
			Quit();
		}

		if (event.type == EventType::kKeyDown) {
			if (event.key == SDLK_ESCAPE) {
				Quit();
			}
			if (event.key == SDLK_F1) {
				engine::ToggleMusic();
			}
		}

		current_scene_->events(event);
	}
}

void Game::Run() {
	while (running_) {
		clock_.Tick(120);

		Events();

		Update();

		Draw();

		engine::graphics::Update();

		previous_scene_.reset();
	}
}

void Game::ChangeScene(std::unique_ptr<Scene> scene) {
	previous_scene_ = std::move(current_scene_);
	current_scene_ = std::move(scene);
}

void Game::Quit() { running_ = false; }

}
