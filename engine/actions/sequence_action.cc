#include "engine/actions/sequence_action.h"

namespace engine {

std::string SequenceAction::Execute(Actor* actor, GameMap* map) {
	std::string result;
	for (Action* action : actions_) {
		std::string part = action->Execute(actor, map);
		if (part.empty()) {
			continue;
		}
		if (!result.empty()) {
			result += "\n";
		}
		result += part;
	}
	return result;
}

}  // namespace engine
