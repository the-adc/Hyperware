#ifndef HYPER_GAME_GAME_H_
#define HYPER_GAME_GAME_H_

#include "engine/clock.h"
#include "engine/graphics.h"
#include "engine/vec2.h"

namespace game {

// A class used to represent a Game.
//
// Author: Yash Mahmud
class Game {
public:
	// Author: Yash Mahmud
	//
	// Constructs all the necessary attributes for the game object.
	Game();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	~Game();

	// Author: Yash Mahmud
	//
	// Draws the game on the screen.
	void Draw();

	// Author: Yash Mahmud
	//
	// Updates the state of the game.
	void Update();

	// Author: Yash Mahmud
	//
	// Handles the game events.
	void Events();

	// Author: Yash Mahmud
	//
	// Runs the game.
	void Run();

	// Author: Yash Mahmud
	//
	// Quits the game.
	void Quit();

	bool running() const { return running_; }
	const engine::Vec2& screen_size() const { return screen_size_; }
	const engine::Vec2& screen_center() const { return screen_center_; }
	engine::Surface* screen() const { return screen_; }

private:
	bool running_;
	engine::Vec2 screen_size_;
	engine::Vec2 screen_center_;
	engine::Surface* screen_;
	engine::Clock clock_;
};

}  // namespace game

#endif  // HYPER_GAME_GAME_H_
