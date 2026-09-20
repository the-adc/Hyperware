#ifndef HYPER_ENGINE_ACTORS_ACTOR_H_
#define HYPER_ENGINE_ACTORS_ACTOR_H_

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>

#include <map>
#include <memory>
#include <unordered_map>

#include "engine/actions/action.h"
#include "engine/actions/action_list.h"
#include "engine/actors/attributes/actor_attribute.h"
#include "engine/actors/attributes/actor_attribute_operations.h"
#include "engine/actors/behaviour.h"
#include "engine/assets.h"
#include "engine/capabilities/capability.h"
#include "engine/color.h"
#include "engine/display/drawable.h"
#include "engine/game_entity.h"

namespace engine {

class GameMap;

class Actor : public GameEntity, public Drawable {
public:
	// kind is the first capability
	explicit Actor(const Capability& kind);
	~Actor() override = default;

	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;

	// events
	virtual void Create() {}
	virtual void Step() {}
	void Draw(Display* display) override;
	virtual void AnimationEnd() {}

	bool HasAttribute(const Capability& name) const;
	void AddAttribute(const Capability& name,
		std::unique_ptr<ActorAttribute> attribute);
	void ModifyAttribute(const Capability& name,
		ActorAttributeOperations operation, double value);
	void ModifyAttributeMaximum(const Capability& name,
		ActorAttributeOperations operation, double value);
	double GetAttribute(const Capability& name) const;
	double GetAttributeMaximum(const Capability& name) const;

	// lowest priority number is asked first
	void AddBehaviour(int priority, std::unique_ptr<Behaviour> behaviour);

	// what the actor does this step, the map runs it right after Step
	virtual Action* PlayTurn(GameMap* map);

	// what other_actor can do to this actor when they touch
	virtual ActionList AllowableActions(Actor* other_actor, GameMap* map);

	void DrawSelf(Display* display);

	// returns true when the animation loops
	bool AdvanceAnimation();

	// the map calls this when it adds the actor
	void Initialize(GameMap* map, int id, double x, double y, int depth);

	void RecordPreviousPosition();
	void RestorePreviousPosition();
	void ApplyMotion();

	// skipped from now on, the map removes it at the end of the phase
	void MarkDestroyed() { destroyed_ = true; }
	bool destroyed() const { return destroyed_; }

	int id() const { return id_; }
	GameMap* map() const { return map_; }

	double x() const { return position_.x; }
	void set_x(double value) { position_.x = value; }
	double y() const { return position_.y; }
	void set_y(double value) { position_.y = value; }
	double xprevious() const { return previous_.x; }
	double yprevious() const { return previous_.y; }
	double xstart() const { return start_.x; }
	double ystart() const { return start_.y; }

	assets::SpriteId sprite_index() const { return sprite_index_; }
	void set_sprite_index(assets::SpriteId value) { sprite_index_ = value; }
	double image_index() const { return image_index_; }
	void set_image_index(double value) { image_index_ = value; }
	double image_speed() const { return image_speed_; }
	void set_image_speed(double value) { image_speed_ = value; }
	double image_angle() const { return image_angle_; }
	void set_image_angle(double value) { image_angle_ = value; }
	double image_xscale() const { return image_xscale_; }
	void set_image_xscale(double value) { image_xscale_ = value; }
	double image_yscale() const { return image_yscale_; }
	void set_image_yscale(double value) { image_yscale_ = value; }
	double image_alpha() const { return image_alpha_; }
	void set_image_alpha(double value) { image_alpha_ = value; }
	Color image_blend() const { return image_blend_; }
	void set_image_blend(Color value) { image_blend_ = value; }

	int depth() const override { return depth_; }
	void set_depth(int value) { depth_ = value; }
	bool solid() const { return solid_; }
	void set_solid(bool value) { solid_ = value; }
	bool visible() const override { return visible_; }
	void set_visible(bool value) { visible_ = value; }

	double speed() const { return speed_; }
	void set_speed(double value) { speed_ = value; }
	double direction() const { return direction_.asDegrees(); }
	void set_direction(double value);
	// keeps an angle between 0 and 360
	static double NormalizeDirection(double value);

	double Hspeed() const;
	double Vspeed() const;

	// collision box in the room, both ends included
	double BboxLeft() const;
	double BboxRight() const;
	double BboxTop() const;
	double BboxBottom() const;

private:
	// speed and direction as an x, y step
	sf::Vector2<double> Velocity() const;

	std::unordered_map<Capability, std::unique_ptr<ActorAttribute>> attributes_;
	std::map<int, std::unique_ptr<Behaviour>> behaviours_;

	GameMap* map_ = nullptr;
	int id_ = 0;

	sf::Vector2<double> position_;
	sf::Vector2<double> previous_;
	sf::Vector2<double> start_;

	assets::SpriteId sprite_index_ = assets::kNoSprite;
	double image_index_ = 0.0;
	double image_speed_ = 1.0;
	double image_angle_ = 0.0;
	double image_xscale_ = 1.0;
	double image_yscale_ = 1.0;
	double image_alpha_ = 1.0;
	Color image_blend_ = C_WHITE;

	int depth_ = 0;
	bool solid_ = false;
	bool visible_ = true;
	bool destroyed_ = false;

	double speed_ = 0.0;
	sf::Angle direction_;
};

// true if the collision masks of the two overlap
bool ActorsCollide(const Actor& first, const Actor& second);

}  // namespace engine

#endif  // HYPER_ENGINE_ACTORS_ACTOR_H_
