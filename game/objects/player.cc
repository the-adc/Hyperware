#include "game/objects/player.h"

#include "engine/display/display.h"
#include "engine/input.h"
#include "engine/positions/game_map.h"
#include "engine/utils.h"
#include "game/game.h"
#include "game/objects.h"

namespace game {
namespace {

using Key = sf::Keyboard::Scancode;

// pixels per step, speeds are never per second
constexpr double kSpeed = 4.0;
constexpr double kHalfSize = 16.0;

bool Left() { return input::KeyDirect(Key::A) || input::KeyDirect(Key::Left); }

bool Right() {
	return input::KeyDirect(Key::D) || input::KeyDirect(Key::Right);
}

bool Up() { return input::KeyDirect(Key::W) || input::KeyDirect(Key::Up); }

bool Down() { return input::KeyDirect(Key::S) || input::KeyDirect(Key::Down); }

}  // namespace

Player::Player() : Actor(kPlayer) {}

void Player::Step() {
	double dx = (Right() ? 1.0 : 0.0) - (Left() ? 1.0 : 0.0);
	double dy = (Down() ? 1.0 : 0.0) - (Up() ? 1.0 : 0.0);
	set_x(engine::Util::Clamp(x() + dx * kSpeed, kHalfSize,
		map()->width() - kHalfSize));
	set_y(engine::Util::Clamp(y() + dy * kSpeed, kHalfSize,
		map()->height() - kHalfSize));

	if (input::KeyPressed(Key::Escape)) {
		Quit();
	}
}

// an actor with a sprite and no draw event draws the sprite with DrawSelf()
void Player::Draw(engine::Display* display) {
	display->RectangleColor(x() - kHalfSize, y() - kHalfSize, x() + kHalfSize,
		y() + kHalfSize, engine::C_AQUA, engine::C_AQUA, engine::C_AQUA,
		engine::C_AQUA, false);
}

}  // namespace game
