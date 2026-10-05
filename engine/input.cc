#include "engine/input.h"

#include <SFML/Window/Mouse.hpp>

#include <array>
#include <cstddef>

namespace {

using Keys = std::array<bool, sf::Keyboard::ScancodeCount>;

Keys keys_now{};
Keys keys_before{};
bool mouse_now = false;
double mouse_x = 0.0;
double mouse_y = 0.0;

// if SMFL doesn't recognize the key, it returns false
bool KnownKey(sf::Keyboard::Scancode key) {
	return key != sf::Keyboard::Scancode::Unknown;
}

std::size_t Index(sf::Keyboard::Scancode key) {
	return static_cast<std::size_t>(key);
}

}  // namespace

namespace input {

void BeginStep(engine::Display& display) {
	keys_before = keys_now;

	// nothing is pressed while the window isn't active
	sf::RenderWindow& window = display.window();
	if (!window.hasFocus()) {
		keys_now.fill(false);
		mouse_now = false;
		return;
	}

	for (std::size_t i = 0; i < keys_now.size(); ++i) {
		keys_now[i] = sf::Keyboard::isKeyPressed(
			static_cast<sf::Keyboard::Scancode>(i));
	}
	mouse_now = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	sf::Vector2i position = sf::Mouse::getPosition(window);
	display.WindowToRoom(position.x, position.y, &mouse_x, &mouse_y);
}

bool KeyDirect(sf::Keyboard::Scancode key) {
	return KnownKey(key) && keys_now[Index(key)];
}

bool KeyPressed(sf::Keyboard::Scancode key) {
	return KnownKey(key) && keys_now[Index(key)] && !keys_before[Index(key)];
}

bool MouseLeft() { return mouse_now; }

double MouseX() { return mouse_x; }

double MouseY() { return mouse_y; }

}  // namespace input
