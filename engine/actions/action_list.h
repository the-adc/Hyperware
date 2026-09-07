#ifndef HYPER_ENGINE_ACTIONS_ACTION_LIST_H_
#define HYPER_ENGINE_ACTIONS_ACTION_LIST_H_

#include <cstddef>
#include <vector>

#include "engine/actions/action.h"

namespace engine {

// List maintaining the order for execution of the Actions
class ActionList {
public:
	ActionList() = default;

	void Add(Action* action);
	void Add(const ActionList& actions);

	bool empty() const { return actions_.empty(); }
	std::size_t size() const { return actions_.size(); }

	std::vector<Action*>::const_iterator begin() const {
		return actions_.begin();
	}
	std::vector<Action*>::const_iterator end() const { return actions_.end(); }

private:
	std::vector<Action*> actions_;
};

}  // namespace engine

#endif  // HYPER_ENGINE_ACTIONS_ACTION_LIST_H_
