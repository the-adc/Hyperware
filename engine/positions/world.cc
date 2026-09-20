#include "engine/positions/world.h"

#include <SFML/System.hpp>

#include <utility>

#include "engine/display/display.h"

namespace engine {

World::World(Display* display) : display_(display) {}

GameMap* World::AddGameMap(std::unique_ptr<GameMap> game_map) {
	GameMap* added = game_map.get();
	added->world_ = this;
	game_maps_.push_back(std::move(game_map));
	return added;
}

void World::RegisterCollision(const Capability& first,
	const Capability& second) {
	collision_pairs_.push_back(std::make_pair(first, second));
}

void World::StartMap(GameMap* game_map) {
	if (active_map_) {
		active_map_->Reset();
	}
	active_map_ = game_map;
	active_map_->Reset();

	display_->SetRoomSurface(game_map->view_width(), game_map->view_height(),
		game_map->port_width(), game_map->port_height());

	for (const GameMap::Placement& placement : game_map->placements()) {
		active_map_->AddActor(placement.create(), placement.x, placement.y,
			placement.depth);
	}
}

void World::GotoMap(GameMap* game_map) { pending_map_ = game_map; }

void World::ApplyMapChange() {
	if (!pending_map_) {
		return;
	}
	GameMap* next = pending_map_;
	pending_map_ = nullptr;
	StartMap(next);
}

void World::Run() {
	sf::Clock clock;
	const sf::Time step = sf::microseconds(1000000 / 60);
	sf::Time next = step;

	while (StillRunning()) {
		Frame();

		sf::Time now = clock.getElapsedTime();
		if (now < next) {
			sf::sleep(next - now);
			next += step;
		} else {
			// too slow for this frame, don't try to catch up
			next = now + step;
		}
	}
}

void World::Frame() {
	BeginFrame();

	Step();

	display_->BeginFrame();
	Draw();
	display_->EndFrame();

	ApplyMapChange();
}

void World::Step() {
	if (active_map_) {
		active_map_->Step(collision_pairs_);
	}
}

void World::Draw() {
	if (active_map_) {
		active_map_->Draw(display_);
	}
}

void World::Clear() {
	if (active_map_) {
		active_map_->Reset();
	}
}

}  // namespace engine
