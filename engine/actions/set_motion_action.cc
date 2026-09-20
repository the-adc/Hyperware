#include "engine/actions/set_motion_action.h"

#include "engine/actors/actor.h"

namespace engine {

std::string SetMotionAction::Execute(Actor* actor, GameMap* /*map*/) {
	if (has_direction_) {
		actor->set_direction(direction_);
	}
	actor->set_speed(speed_);
	return "";
}

}  // namespace engine
