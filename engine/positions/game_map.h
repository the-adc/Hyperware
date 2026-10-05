#ifndef HYPER_ENGINE_POSITIONS_GAME_MAP_H_
#define HYPER_ENGINE_POSITIONS_GAME_MAP_H_

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "engine/actions/action.h"
#include "engine/actions/action_list.h"
#include "engine/actors/actor.h"
#include "engine/assets.h"
#include "engine/capabilities/capability.h"

namespace engine {

class Display;
class World;

// This is kinda like rooms in GameMaker, or Scenes in Unity
class GameMap {
public:
	struct Background {
		assets::SpriteId sprite = assets::kNoSprite;
		double hspeed = 0.0;
		double vspeed = 0.0;
		int depth = 0;
	};

	// an actor that exists when the map starts
	struct Placement {
		std::function<std::unique_ptr<Actor>()> create;
		double x = 0.0;
		double y = 0.0;
		int depth = 0;
	};

	// actors of the first kind have a collision event with the second kind
	using CollisionPair = std::pair<Capability, Capability>;

	GameMap(std::string name, int width, int height);

	GameMap(const GameMap&) = delete;
	GameMap& operator=(const GameMap&) = delete;

	const std::string& name() const { return name_; }
	int width() const { return width_; }
	int height() const { return height_; }

	// the view is the camera, the port is what it gets scaled to
	void SetView(int view_width, int view_height) {
		view_width_ = view_width;
		view_height_ = view_height;
	}
	void SetPort(int port_width, int port_height) {
		port_width_ = port_width;
		port_height_ = port_height;
	}
	int view_width() const { return view_width_; }
	int view_height() const { return view_height_; }
	int port_width() const { return port_width_; }
	int port_height() const { return port_height_; }

	void AddBackground(const Background& background) {
		backgrounds_.push_back(background);
	}
	// created in the order they were placed
	template <typename T>
	void AddPlacement(double x, double y, int depth) {
		placements_.push_back(
			{[] { return std::make_unique<T>(); }, x, y, depth});
	}
	const std::vector<Placement>& placements() const { return placements_; }

	World* world() const { return world_; }

	// runs the Create event before it returns
	Actor* AddActor(std::unique_ptr<Actor> actor, double x, double y, int depth);

	template <typename T, typename... Args>
	T* AddActor(double x, double y, int depth, Args&&... args) {
		return static_cast<T*>(AddActor(
			std::make_unique<T>(std::forward<Args>(args)...), x, y, depth));
	}

	// removed at the end of the phase
	void DestroyActor(Actor* actor);

	// an action that lives until the next step, for ones that have state
	template <typename T, typename... Args>
	T* NewAction(Args&&... args) {
		T* action = new T(std::forward<Args>(args)...);
		frame_actions_.emplace_back(action);
		return action;
	}

	int CountActors(const Capability& kind) const;
	Actor* FindActor(const Capability& kind) const;
	Actor* NearestActor(double x, double y, const Capability& kind) const;

	// like "with (obj)". Check destroyed() if the loop destroys actors.
	std::vector<Actor*> ActorsWith(const Capability& kind) const;

	// true if the actor wouldn't touch a solid actor at x, y
	bool PlaceFree(Actor* actor, double x, double y) const;

	void Step(const std::vector<CollisionPair>& collision_pairs);

	// highest depth first, then in creation order
	void Draw(Display* display);

	// destroys all the actors and puts the backgrounds back
	void Reset();

private:
	friend class World;

	void StepAnimations();
	void StepEvents();
	void StepCollisions(const std::vector<CollisionPair>& collision_pairs);
	void ScrollBackgrounds();

	void PurgeDestroyed();
	void Report(const std::string& result);
	// the live actors in creation order
	std::vector<Actor*> Snapshot() const;

	std::string name_;
	int width_;
	int height_;
	int view_width_ = 640;
	int view_height_ = 480;
	int port_width_ = 640;
	int port_height_ = 480;

	std::vector<Background> backgrounds_;
	std::vector<Placement> placements_;

	World* world_ = nullptr;
	std::vector<std::unique_ptr<Actor>> actors_;
	std::vector<std::unique_ptr<Action>> frame_actions_;

	// how far each background has scrolled
	std::vector<double> background_x_;
	std::vector<double> background_y_;
};

}  // namespace engine

#endif  // HYPER_ENGINE_POSITIONS_GAME_MAP_H_
