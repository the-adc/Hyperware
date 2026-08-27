#include "ui/UIButton.h"

#include "ui/Draw.h"
#include "ui/UIManager.h"

UIButton::UIButton(const Rect &relative_rect, const std::string &text, UIManager *manager,
                   UIElement *container, const std::string &theme_block)
    : UIElement(relative_rect, manager, container), text(text), theme_block(theme_block),
      held(false) {}

bool UIButton::process_event(const Event &event) {
    Rect area = rect();

    if (event.type == EVENT_MOUSEBUTTONDOWN && event.button == SDL_BUTTON_LEFT) {
        if (area.collidepoint(event.x, event.y)) {
            held = true;
            return true;
        }
        return false;
    }

    if (event.type == EVENT_MOUSEBUTTONUP && event.button == SDL_BUTTON_LEFT) {
        bool was_held = held;
        held = false;
        if (was_held && area.collidepoint(event.x, event.y)) {
            Event pressed(EVENT_UI_BUTTON_PRESSED);
            pressed.ui_element = this;
            events::post(pressed);
            return true;
        }
    }

    return false;
}

void UIButton::draw(Surface &surface) {
    Rect area = rect();

    int mouse_x = 0;
    int mouse_y = 0;
    Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    bool hovered = area.collidepoint(mouse_x, mouse_y);
    bool pressed = held && hovered && (buttons & SDL_BUTTON(SDL_BUTTON_LEFT));

    Color normal_bg = manager->theme.colour(theme_block, "normal_bg", Color(0x45, 0x49, 0x4e));
    Color hovered_bg = manager->theme.colour(theme_block, "hovered_bg", Color(0x35, 0x39, 0x3e));
    Color active_bg = manager->theme.colour(theme_block, "active_bg", Color(0x19, 0x37, 0x54));
    Color border = manager->theme.colour(theme_block, "normal_border", Color(0xDD, 0xDD, 0xDD));
    Color normal_text = manager->theme.colour(theme_block, "normal_text", Color(0xc5, 0xcb, 0xd8));
    Color hovered_text = manager->theme.colour(theme_block, "hovered_text", Color(255, 255, 255));

    Color background = normal_bg;
    Color foreground = normal_text;
    if (pressed) {
        background = active_bg;
        foreground = hovered_text;
    } else if (hovered) {
        background = hovered_bg;
        foreground = hovered_text;
    }

    int radius = manager->theme.misc_int(theme_block, "shape_corner_radius", 2);
    int border_width = manager->theme.misc_int(theme_block, "border_width", 1);

    uidraw::rounded_rect_with_border(surface, area, radius, border_width, background, border);
    uidraw::blit_text(surface, manager->button_font(), text, foreground, area, "center", "center");
}
