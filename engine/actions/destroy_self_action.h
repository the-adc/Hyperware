#ifndef HYPER_ENGINE_ACTIONS_DESTROY_SELF_ACTION_H_
#define HYPER_ENGINE_ACTIONS_DESTROY_SELF_ACTION_H_

#include <string>

#include "engine/actions/action.h"

namespace engine {

// The actor removes itself from the map.
class DestroySelfAction : public Action {
public:
	static DestroySelfAction* Instance();

	std::string Execute(Actor* actor, GameMap* map) override;
};

}  // namespace engine

#endif  // HYPER_ENGINE_ACTIONS_DESTROY_SELF_ACTION_H_
