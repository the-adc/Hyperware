#ifndef HYPER_ENGINE_ACTIONS_ACTION_H_
#define HYPER_ENGINE_ACTIONS_ACTION_H_

#include <string>

namespace engine {

class Actor;
class GameMap;

class Action {
public:
	virtual ~Action() = default;

	// Returns action in the form of a string
	virtual std::string Execute(Actor* actor, GameMap* map) = 0;
};

}  // namespace engine

#endif  // HYPER_ENGINE_ACTIONS_ACTION_H_
