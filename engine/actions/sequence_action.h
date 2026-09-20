#ifndef HYPER_ENGINE_ACTIONS_SEQUENCE_ACTION_H_
#define HYPER_ENGINE_ACTIONS_SEQUENCE_ACTION_H_

#include <string>
#include <utility>
#include <vector>

#include "engine/actions/action.h"

namespace engine {

// Runs several actions in a frame, a sequence might i say
class SequenceAction : public Action {
public:
	explicit SequenceAction(std::vector<Action*> actions)
		: actions_(std::move(actions)) {}

	std::string Execute(Actor* actor, GameMap* map) override;

private:
	std::vector<Action*> actions_;
};

}  // namespace engine

#endif  // HYPER_ENGINE_ACTIONS_SEQUENCE_ACTION_H_
