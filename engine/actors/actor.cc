#include "engine/actors/actor.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <cmath>
#include <utility>

#include "engine/actions/do_nothing_action.h"
#include "engine/display/display.h"

namespace engine {
namespace {

using Position = sf::Vector2<double>;
using Box = sf::Rect<double>;

double Right(const Box& box) { return box.position.x + box.size.x; }
double Bottom(const Box& box) { return box.position.y + box.size.y; }

// between 0 and 360
sf::Angle NormalizedAngle(double degrees) {
	return sf::degrees(static_cast<float>(degrees)).wrapUnsigned();
}

// sprite pixels (from the origin) to an offset from the actor
// gamemaker turns counter-clockwise and SFML clockwise
sf::Transform SpriteTransform(const Actor& actor) {
	sf::Transform transform;
	transform.rotate(sf::degrees(static_cast<float>(-actor.image_angle())));
	transform.scale({static_cast<float>(actor.image_xscale()),
		static_cast<float>(actor.image_yscale())});
	return transform;
}

Position SpriteToRoom(const Actor& actor, double sprite_x, double sprite_y) {
	sf::Vector2f offset = SpriteTransform(actor).transformPoint(
		{static_cast<float>(sprite_x), static_cast<float>(sprite_y)});
	return Position(offset);
}

// a point in the room to a pixel of the image
Position RoomToSprite(const Actor& actor, double room_x, double room_y) {
	const assets::Sprite& data = assets::GetSprite(actor.sprite_index());
	Position origin(data.xorigin, data.yorigin);
	if (actor.image_xscale() == 0.0 || actor.image_yscale() == 0.0) {
		return origin;  // a sprite with no size can't be turned around
	}

	sf::Vector2f offset(static_cast<float>(room_x - actor.x()),
		static_cast<float>(room_y - actor.y()));
	sf::Transform inverse = SpriteTransform(actor).getInverse();
	return origin + Position(inverse.transformPoint(offset));
}

// a straight box around the collision box, even if the sprite is rotated
Box Bounds(const Actor& actor) {
	const assets::Sprite& data = assets::GetSprite(actor.sprite_index());
	if (data.frames.empty()) {
		return Box();
	}

	double left = data.bbox_left - data.xorigin;
	double top = data.bbox_top - data.yorigin;
	double right = data.bbox_right - data.xorigin;
	double bottom = data.bbox_bottom - data.yorigin;

	Position corners[] = {
		SpriteToRoom(actor, left, top),
		SpriteToRoom(actor, right, top),
		SpriteToRoom(actor, right, bottom),
		SpriteToRoom(actor, left, bottom),
	};

	Position low = corners[0];
	Position high = corners[0];
	for (const Position& corner : corners) {
		low = {std::min(low.x, corner.x), std::min(low.y, corner.y)};
		high = {std::max(high.x, corner.x), std::max(high.y, corner.y)};
	}

	Position here(actor.x(), actor.y());
	return Box(low + here, high - low);
}

// checks a point of the room against the precise mask
bool MaskHit(const Actor& actor, double room_x, double room_y) {
	const assets::Sprite& data = assets::GetSprite(actor.sprite_index());
	if (!data.IsPrecise() || data.frames.empty()) {
		return true;  // a rectangle is solid everywhere
	}

	int count = data.FrameCount();
	int frame = static_cast<int>(std::floor(actor.image_index())) % count;
	if (frame < 0) {
		frame += count;
	}
	const std::vector<std::uint8_t>& mask = data.frames[frame].mask;
	if (mask.empty()) {
		return true;
	}

	Position local = RoomToSprite(actor, room_x, room_y);
	int px = static_cast<int>(std::floor(local.x));
	int py = static_cast<int>(std::floor(local.y));
	if (px < 0 || py < 0 || px >= data.width || py >= data.height) {
		return false;
	}
	return mask[py * data.width + px] != 0;
}

}  // namespace

Actor::Actor(const Capability& kind) { AddCapability(kind); }

bool Actor::HasAttribute(const Capability& name) const {
	return attributes_.count(name) != 0;
}

void Actor::AddAttribute(const Capability& name,
	std::unique_ptr<ActorAttribute> attribute) {
	attributes_[name] = std::move(attribute);
}

void Actor::ModifyAttribute(const Capability& name,
	ActorAttributeOperations operation, double value) {
	ActorAttribute* attribute = attributes_.at(name).get();
	switch (operation) {
		case ActorAttributeOperations::kIncrease:
			attribute->Increase(value);
			break;
		case ActorAttributeOperations::kDecrease:
			attribute->Decrease(value);
			break;
		case ActorAttributeOperations::kUpdate:
			attribute->Update(value);
			break;
	}
}

void Actor::ModifyAttributeMaximum(const Capability& name,
	ActorAttributeOperations operation, double value) {
	ActorAttribute* attribute = attributes_.at(name).get();
	switch (operation) {
		case ActorAttributeOperations::kIncrease:
			attribute->IncreaseMaximum(value);
			break;
		case ActorAttributeOperations::kDecrease:
			attribute->DecreaseMaximum(value);
			break;
		case ActorAttributeOperations::kUpdate:
			attribute->UpdateMaximum(value);
			break;
	}
}

double Actor::GetAttribute(const Capability& name) const {
	return attributes_.at(name)->Get();
}

double Actor::GetAttributeMaximum(const Capability& name) const {
	return attributes_.at(name)->GetMaximum();
}

void Actor::AddBehaviour(int priority, std::unique_ptr<Behaviour> behaviour) {
	behaviours_[priority] = std::move(behaviour);
}

Action* Actor::PlayTurn(GameMap* map) {
	for (const auto& entry : behaviours_) {
		Action* action = entry.second->GetAction(this, map);
		if (action) {
			return action;
		}
	}
	return DoNothingAction::Instance();
}

ActionList Actor::AllowableActions(Actor* /*other_actor*/, GameMap* /*map*/) {
	return ActionList();
}

void Actor::Initialize(GameMap* map, int id, double x, double y, int depth) {
	map_ = map;
	id_ = id;
	position_ = previous_ = start_ = Position(x, y);
	depth_ = depth;
}

void Actor::RecordPreviousPosition() { previous_ = position_; }

void Actor::RestorePreviousPosition() { position_ = previous_; }

void Actor::ApplyMotion() {
	if (speed_ == 0.0) {
		return;
	}
	position_ += Velocity();
}

void Actor::set_direction(double value) { direction_ = NormalizedAngle(value); }

double Actor::NormalizeDirection(double value) {
	return NormalizedAngle(value).asDegrees();
}

// the angle is negative because gamemaker turns counter clockwise
Position Actor::Velocity() const { return Position(speed_, -direction_); }

double Actor::Hspeed() const { return Velocity().x; }

double Actor::Vspeed() const { return Velocity().y; }

double Actor::BboxLeft() const { return Bounds(*this).position.x; }

double Actor::BboxRight() const { return Right(Bounds(*this)); }

double Actor::BboxTop() const { return Bounds(*this).position.y; }

double Actor::BboxBottom() const { return Bottom(Bounds(*this)); }

void Actor::Draw(Display* display) { DrawSelf(display); }

void Actor::DrawSelf(Display* display) {
	if (sprite_index_ == assets::kNoSprite) {
		return;
	}
	display->SpriteExt(sprite_index_, image_index_, x(), y(), image_xscale_,
		image_yscale_, image_angle_, image_blend_, image_alpha_);
}

bool Actor::AdvanceAnimation() {
	if (sprite_index_ == assets::kNoSprite || image_speed_ == 0.0) {
		return false;
	}
	const assets::Sprite& data = assets::GetSprite(sprite_index_);
	int count = data.FrameCount();
	if (count <= 0) {
		return false;
	}

	// using 60 since thats the base framerate
	image_index_ += image_speed_ * data.fps / 60.0;
	if (image_index_ >= count) {
		image_index_ = std::fmod(image_index_, static_cast<double>(count));
		return true;
	}
	return false;
}

bool ActorsCollide(const Actor& first, const Actor& second) {
	const assets::Sprite& first_sprite = assets::GetSprite(first.sprite_index());
	const assets::Sprite& second_sprite =
		assets::GetSprite(second.sprite_index());
	if (first_sprite.frames.empty() || second_sprite.frames.empty()) {
		return false;
	}

	// the edges count as touching, so findIntersection can't be used
	Box a = Bounds(first);
	Box b = Bounds(second);
	if (Right(a) < b.position.x || Right(b) < a.position.x ||
		Bottom(a) < b.position.y || Bottom(b) < a.position.y) {
		return false;
	}
	if (!first_sprite.IsPrecise() && !second_sprite.IsPrecise()) {
		return true;
	}

	// only the pixels where the boxes overlap, and only precise masks matter
	int left = static_cast<int>(std::ceil(std::max(a.position.x, b.position.x)));
	int top = static_cast<int>(std::ceil(std::max(a.position.y, b.position.y)));
	int right = static_cast<int>(std::floor(std::min(Right(a), Right(b))));
	int bottom = static_cast<int>(std::floor(std::min(Bottom(a), Bottom(b))));

	for (int py = top; py <= bottom; ++py) {
		for (int px = left; px <= right; ++px) {
			if (MaskHit(first, px, py) && MaskHit(second, px, py)) {
				return true;
			}
		}
	}
	return false;
}

}  // namespace engine
