#ifndef HYPER_ENGINE_POSITIONS_WORLD_H_
#define HYPER_ENGINE_POSITIONS_WORLD_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "engine/actors/actor.h"
#include "engine/capabilities/capability.h"
#include "engine/positions/game_map.h"

namespace engine {

class Display;

// Fun fact, the original code had a jojo reference not by me

// This handles all the GameMaps and collision and other stuff, this is like
// the entire game
class World {
public:
	virtual ~World() = default;

	// the display has to outlive the world
	explicit World(Display* display);

	World(const World&) = delete;
	World& operator=(const World&) = delete;

	// the world owns the map from now on
	GameMap* AddGameMap(std::unique_ptr<GameMap> game_map);

	// actors with first get a collision event when they touch second
	void RegisterCollision(const Capability& first, const Capability& second);

	void StartMap(GameMap* game_map);
	// happens at the end of the frame, like room_goto
	void GotoMap(GameMap* game_map);
	bool MapChangePending() const { return pending_map_ != nullptr; }
	void ApplyMapChange();

	GameMap* active_map() const { return active_map_; }
	// what the actions of the last step said
	const std::vector<std::string>& frame_messages() const {
		return frame_messages_;
	}
	Display* display() const { return display_; }

	// 60 frames a second until StillRunning() is false
	void Run();

	virtual bool StillRunning() const = 0;

	// step, draw, and then change map if one was asked for
	void Frame();

	void Step();
	void Draw();
	void Clear();

protected:
	// start of every frame, before the step
	virtual void BeginFrame() {}

private:
	friend class GameMap;

	int NextActorId() { return next_actor_id_++; }

	Display* display_;
	std::vector<std::unique_ptr<GameMap>> game_maps_;
	GameMap* active_map_ = nullptr;
	GameMap* pending_map_ = nullptr;

	std::vector<GameMap::CollisionPair> collision_pairs_;

	std::vector<std::string> frame_messages_;

	int next_actor_id_ = 1;
};

}  // namespace engine

#endif  // HYPER_ENGINE_POSITIONS_WORLD_H_
