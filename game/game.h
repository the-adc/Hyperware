#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include <memory>

#include "engine/clock.h"
#include "engine/graphics.h"
#include "engine/vec2.h"
#include "game/scene.h"

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
	// Changes the current scene to the provided scene.
	void ChangeScene(std::unique_ptr<Scene> scene);

	// Author: Yash Mahmud
	//
	// Quits the game.
	void Quit();

	bool running() const { return running_; }
	const engine::Vec2& screen_size() const { return screen_size_; }
	const engine::Vec2& screen_center() const { return screen_center_; }
	engine::Surface* screen() const { return screen_; }
	Scene* current_scene() const { return current_scene_.get(); }
	Scene* previous_scene() const { return previous_scene_.get(); }

private:
	bool running_;
	engine::Vec2 screen_size_;
	engine::Vec2 screen_center_;
	engine::Surface* screen_;
	std::unique_ptr<Scene> current_scene_;
	std::unique_ptr<Scene> previous_scene_;
	engine::Clock clock_;
};

}

#endif
