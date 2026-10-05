#ifndef HYPER_GAME_OBJECTS_PLAYER_H_
#define HYPER_GAME_OBJECTS_PLAYER_H_

#include "engine/actors/actor.h"

namespace game {

// A square you move with WASD or the arrow keys, Esc quits the game
// only meant for demonstration purposes
class Player : public engine::Actor {
public:
	Player();

	void Step() override;
	void Draw(engine::Display* display) override;
};

}  // namespace game

#endif  // HYPER_GAME_OBJECTS_PLAYER_H_
