#ifndef HYPER_ENGINE_INPUT_H_
#define HYPER_ENGINE_INPUT_H_

#include <SFML/Window/Keyboard.hpp>

#include "engine/display/display.h"

namespace input {

void BeginStep(engine::Display& display);

bool KeyDirect(sf::Keyboard::Scancode key);
bool KeyPressed(sf::Keyboard::Scancode key);
bool MouseLeft();

double MouseX();
double MouseY();

}  // namespace input

#endif  // HYPER_ENGINE_INPUT_H_
