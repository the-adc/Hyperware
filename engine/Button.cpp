#include "engine/Button.h"

Button::Button(double x, double y, const std::string &image, Vec2 scale, double angle)
    : Sprite(x, y, image, scale, angle) {
    click_state = ClickState::RELEASE;
    clicked = false;
}

void Button::draw(Surface &surface) {
    Sprite::draw(surface);
}

bool Button::update() {
    int mouse_x = 0;
    int mouse_y = 0;
    Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

    if (rect.collidepoint(mouse_x, mouse_y)) {
        clicked = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

        if (clicked) {
            if (click_state == ClickState::RELEASE) {
                click_state = ClickState::CLICK;
                on_click();
                return clicked;
            }

            if (click_state == ClickState::CLICK) {
                click_state = ClickState::HOLD;
            }

            if (click_state == ClickState::HOLD) {
                on_hold();
            }
        } else {
            if (!(click_state == ClickState::RELEASE)) {
                click_state = ClickState::RELEASE;
                on_release();
            }
        }
    }

    return clicked;
}
