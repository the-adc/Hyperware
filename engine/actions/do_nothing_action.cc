#include "engine/actions/do_nothing_action.h"

namespace engine {

DoNothingAction* DoNothingAction::Instance() {
	static DoNothingAction* instance = new DoNothingAction;
	return instance;
}

std::string DoNothingAction::Execute(Actor* /*actor*/, GameMap* /*map*/) {
	return "";
}

}  // namespace engine
