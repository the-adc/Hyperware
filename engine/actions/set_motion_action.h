#ifndef HYPER_ENGINE_ACTIONS_SET_MOTION_ACTION_H_
#define HYPER_ENGINE_ACTIONS_SET_MOTION_ACTION_H_

#include <string>

#include "engine/actions/action.h"

namespace engine {

// name pretty self explanatory tbh
class SetMotionAction : public Action {
public:
	// the direction stays the same
	explicit SetMotionAction(double speed)
		: speed_(speed), has_direction_(false) {}
	SetMotionAction(double speed, double direction)
		: speed_(speed), direction_(direction), has_direction_(true) {}

	std::string Execute(Actor* actor, GameMap* map) override;

private:
	double speed_;
	double direction_ = 0.0;
	bool has_direction_;
};

}  // namespace engine

#endif  // HYPER_ENGINE_ACTIONS_SET_MOTION_ACTION_H_
