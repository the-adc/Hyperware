#include "engine/positions/game_map.h"

#include <algorithm>

#include "engine/display/display.h"
#include "engine/positions/world.h"
#include "engine/utils.h"

namespace engine {

GameMap::GameMap(std::string name, int width, int height)
	: name_(std::move(name)), width_(width), height_(height) {}

Actor* GameMap::AddActor(std::unique_ptr<Actor> actor, double x, double y,
	int depth) {
	Actor* added = actor.get();
	added->Initialize(this, world_->NextActorId(), x, y, depth);

	actors_.push_back(std::move(actor));
	// run Create before instance_create_depth returns
	added->Create();
	return added;
}

void GameMap::DestroyActor(Actor* actor) {
	if (actor) {
		actor->MarkDestroyed();
	}
}

int GameMap::CountActors(const Capability& kind) const {
	int count = 0;
	for (const std::unique_ptr<Actor>& actor : actors_) {
		if (!actor->destroyed() && actor->HasCapability(kind)) {
			++count;
		}
	}
	return count;
}

Actor* GameMap::FindActor(const Capability& kind) const {
	for (const std::unique_ptr<Actor>& actor : actors_) {
		if (!actor->destroyed() && actor->HasCapability(kind)) {
			return actor.get();
		}
	}
	return nullptr;
}

Actor* GameMap::NearestActor(double x, double y, const Capability& kind) const {
	Actor* nearest = nullptr;
	double best = 0.0;
	for (const std::unique_ptr<Actor>& actor : actors_) {
		Actor* candidate = actor.get();
		if (candidate->destroyed() || !candidate->HasCapability(kind)) {
			continue;
		}
		double distance =
			Util::PointDistance(x, y, candidate->x(), candidate->y());
		if (!nearest || distance < best) {
			nearest = candidate;
			best = distance;
		}
	}
	return nearest;
}

std::vector<Actor*> GameMap::ActorsWith(const Capability& kind) const {
	std::vector<Actor*> found;
	for (const std::unique_ptr<Actor>& actor : actors_) {
		if (!actor->destroyed() && actor->HasCapability(kind)) {
			found.push_back(actor.get());
		}
	}
	return found;
}

bool GameMap::PlaceFree(Actor* actor, double x, double y) const {
	double saved_x = actor->x();
	double saved_y = actor->y();
	actor->set_x(x);
	actor->set_y(y);

	bool free = true;
	for (const std::unique_ptr<Actor>& other : actors_) {
		if (other.get() == actor || other->destroyed() || !other->solid()) {
			continue;
		}
		if (ActorsCollide(*actor, *other)) {
			free = false;
			break;
		}
	}

	actor->set_x(saved_x);
	actor->set_y(saved_y);
	return free;
}

void GameMap::Step(const std::vector<CollisionPair>& collision_pairs) {
	frame_actions_.clear();
	world_->frame_messages_.clear();

	for (const std::unique_ptr<Actor>& actor : actors_) {
		actor->RecordPreviousPosition();
	}

	StepAnimations();
	StepEvents();

	for (const std::unique_ptr<Actor>& actor : actors_) {
		if (!actor->destroyed()) {
			actor->ApplyMotion();
		}
	}

	StepCollisions(collision_pairs);
	ScrollBackgrounds();
}

// Animation End happens when the sprite loops
void GameMap::StepAnimations() {
	for (Actor* actor : Snapshot()) {
		if (!actor->destroyed() && actor->AdvanceAnimation()) {
			actor->AnimationEnd();
		}
	}
	PurgeDestroyed();
}

// Actors created in here don't get a Step until the next frame
void GameMap::StepEvents() {
	for (Actor* actor : Snapshot()) {
		if (actor->destroyed()) {
			continue;
		}
		actor->Step();
		if (actor->destroyed()) {
			continue;
		}
		// the turn runs now, before the next actor's
		Action* action = actor->PlayTurn(this);
		if (action) {
			Report(action->Execute(actor, this));
		}
	}
	PurgeDestroyed();
}

void GameMap::StepCollisions(
	const std::vector<CollisionPair>& collision_pairs) {
	for (const CollisionPair& pair : collision_pairs) {
		std::vector<Actor*> firsts = ActorsWith(pair.first);
		std::vector<Actor*> seconds = ActorsWith(pair.second);

		for (Actor* a : firsts) {
			for (Actor* b : seconds) {
				if (a->destroyed()) {
					break;
				}
				if (b->destroyed() || a == b || !ActorsCollide(*a, *b)) {
					continue;
				}

				// solid actors go back to where they were before the events
				if (a->solid() || b->solid()) {
					a->RestorePreviousPosition();
					b->RestorePreviousPosition();
				}

				// what b can do to a, run as b
				for (Action* action : a->AllowableActions(b, this)) {
					Report(action->Execute(b, this));
				}
			}
		}
	}
	PurgeDestroyed();
}

void GameMap::ScrollBackgrounds() {
	for (std::size_t i = 0; i < backgrounds_.size(); ++i) {
		background_x_[i] += backgrounds_[i].hspeed;
		background_y_[i] += backgrounds_[i].vspeed;
	}
}

void GameMap::Draw(Display* display) {
	// backgrounds and actors are sorted together
	struct Item {
		int depth;
		int order;
		Actor* actor;      // null for a background
		std::size_t layer;  // the background, if there is no actor
	};

	std::vector<Item> items;
	for (std::size_t i = 0; i < backgrounds_.size(); ++i) {
		items.push_back({backgrounds_[i].depth, -1, nullptr, i});
	}
	for (const std::unique_ptr<Actor>& actor : actors_) {
		if (!actor->destroyed() && actor->visible()) {
			items.push_back({actor->depth(), actor->id(), actor.get(), 0});
		}
	}

	std::stable_sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
		if (a.depth != b.depth) {
			return a.depth > b.depth;
		}
		return a.order < b.order;
	});

	for (const Item& item : items) {
		if (item.actor) {
			item.actor->Draw(display);
		} else {
			display->SpriteTiled(backgrounds_[item.layer].sprite,
				background_x_[item.layer], background_y_[item.layer]);
		}
	}
}

void GameMap::Reset() {
	actors_.clear();
	frame_actions_.clear();
	background_x_.assign(backgrounds_.size(), 0.0);
	background_y_.assign(backgrounds_.size(), 0.0);
}

void GameMap::Report(const std::string& result) {
	if (!result.empty()) {
		world_->frame_messages_.push_back(result);
	}
}

void GameMap::PurgeDestroyed() {
	actors_.erase(std::remove_if(actors_.begin(), actors_.end(),
		[](const std::unique_ptr<Actor>& actor) {
			return actor->destroyed();
		}), actors_.end());
}

std::vector<Actor*> GameMap::Snapshot() const {
	std::vector<Actor*> snapshot;
	for (const std::unique_ptr<Actor>& actor : actors_) {
		snapshot.push_back(actor.get());
	}
	return snapshot;
}

}  // namespace engine
