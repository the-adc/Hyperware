#include "game/game.h"

#include <memory>

#include "engine/app.h"
#include "engine/display/display.h"
#include "engine/positions/game_map.h"
#include "engine/positions/world.h"
#include "game/objects.h"
#include "game/objects/player.h"
#include "game/resources.h"

namespace game {
namespace {

// on the heap and never deleted, the world can't be destroyed trivially
struct State {
	engine::App app;
	engine::GameMap* main_room = nullptr;
};

State& GetState() {
	static State* state = new State;
	return *state;
}

// One 640x480 room with a camera shown at the same size
void BuildRooms() {
	State& state = GetState();

	std::unique_ptr<engine::GameMap> room =
		std::make_unique<engine::GameMap>("main", 640, 480);
	room->SetView(640, 480);
	room->SetPort(640, 480);
	// actors added here
	room->AddPlacement<Player>(320, 240, 0);
	state.main_room = state.app.world()->AddGameMap(std::move(room));
}

}  // namespace

engine::World* GetWorld() { return GetState().app.world(); }

engine::Display* GetDisplay() { return GetState().app.display(); }

engine::GameMap* MainRoom() { return GetState().main_room; }

bool Init() {
	State& state = GetState();

	engine::AppConfig config;
	if (!state.app.Init(config)) {
		return false;
	}

	if (!res::Load(state.app.display(), state.app.assets_root())) {
		return false;
	}

	BuildRooms();

	RegisterCollisions(state.app.world());

	state.app.world()->StartMap(MainRoom());
	return true;
}

void Shutdown() { GetState().app.Shutdown(); }

void Quit() { GetState().app.Quit(); }

}  // namespace game
