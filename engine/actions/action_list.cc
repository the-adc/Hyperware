#include "engine/actions/action_list.h"

namespace engine {

void ActionList::Add(Action* action) {
	if (action) {
		actions_.push_back(action);
	}
}

void ActionList::Add(const ActionList& actions) {
	actions_.insert(actions_.end(), actions.actions_.begin(),
		actions.actions_.end());
}

}  // namespace engine
