#include "engine/button.h"

namespace engine {

Button::Button(double x, double y, const std::string& image, const Vec2& scale,
	double angle) :
	Sprite(x, y, image, scale, angle),
	click_state_(ClickState::kRelease),
	clicked_(false) {}

void Button::Draw(Surface& surface) { Sprite::Draw(surface); }

bool Button::Update() {
	int mouse_x = 0;
	int mouse_y = 0;
	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	if (rect().CollidePoint(mouse_x, mouse_y)) {
		clicked_ = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

		if (clicked_) {
			if (click_state_ == ClickState::kRelease) {
				click_state_ = ClickState::kClick;
				OnClick();
				return clicked_;
			}

			if (click_state_ == ClickState::kClick) {
				click_state_ = ClickState::kHold;
			}

			if (click_state_ == ClickState::kHold) {
				OnHold();
			}
		} else {
			if (click_state_ != ClickState::kRelease) {
				click_state_ = ClickState::kRelease;
				OnRelease();
			}
		}
	}

	return clicked_;
}

}
