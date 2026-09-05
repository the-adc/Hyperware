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
	engine::Util::MixerInit();

	screen_ = &engine::graphics::Screen();
}

Game::~Game() {
	engine::Util::MixerQuit();
	engine::graphics::Quit();
}

void Game::Draw() {
	screen_->Fill(Color(202, 228, 241));
}

void Game::Update() {
	clock_.Tick(60);
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
				engine::Util::ToggleMusic();
			}
		}
	}
}

void Game::Run() {
	while (running_) {
		clock_.Tick(120);

		Events();

		Update();

		Draw();

		engine::graphics::Update();
	}
}

void Game::Quit() { running_ = false; }

}  // namespace game
