#ifndef HYPER_ENGINE_ACTIONS_DO_NOTHING_ACTION_H_
#define HYPER_ENGINE_ACTIONS_DO_NOTHING_ACTION_H_

#include <string>

#include "engine/actions/action.h"

namespace engine {

class DoNothingAction : public Action {
public:

	static DoNothingAction* Instance();

	std::string Execute(Actor* actor, GameMap* map) override;
};

}  // namespace engine

#endif  // HYPER_ENGINE_ACTIONS_DO_NOTHING_ACTION_H_
