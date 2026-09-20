#include "engine/actions/destroy_self_action.h"

#include "engine/positions/game_map.h"

namespace engine {

DestroySelfAction* DestroySelfAction::Instance() {
	static DestroySelfAction* instance = new DestroySelfAction;
	return instance;
}

std::string DestroySelfAction::Execute(Actor* actor, GameMap* map) {
	map->DestroyActor(actor);
	return "";
}

}  // namespace engine
