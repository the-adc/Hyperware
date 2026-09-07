#ifndef HYPER_ENGINE_ACTORS_BEHAVIOUR_H_
#define HYPER_ENGINE_ACTORS_BEHAVIOUR_H_

namespace engine {

class Action;
class Actor;
class GameMap;

// Class for deciding what an Actor wants to do.
class Behaviour {
public:
	virtual ~Behaviour() = default;

	// null if there is nothing to do
	virtual Action* GetAction(Actor* actor, GameMap* map) = 0;
};

}  // namespace engine

#endif  // HYPER_ENGINE_ACTORS_BEHAVIOUR_H_
