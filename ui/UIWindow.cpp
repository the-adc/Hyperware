#include "ui/UIWindow.h"

#include "ui/Draw.h"
#include "ui/UIManager.h"

UIWindow::UIWindow(const Rect &window_rect, UIManager *manager,
                   const std::string &window_display_title, bool resizable, bool draggable)
    : UIElement(window_rect, manager, nullptr), window_display_title(window_display_title),
      resizable(resizable), draggable(draggable), close_held(false) {
    layer = 1;

    shadow_width = manager->theme.misc_int("window", "shadow_width", 15);
    int radius = manager->theme.misc_int("window", "shape_corner_radius", 2);

    shadow = Surface::create(window_rect.w + 2 * shadow_width, window_rect.h + 2 * shadow_width);
    if (!shadow.empty()) {
        shadow.fill(Color(0, 0, 0, 0));
        for (int i = shadow_width; i >= 1; --i) {
            Rect ring(shadow_width - i, shadow_width - i, window_rect.w + 2 * i,
                      window_rect.h + 2 * i);
            uidraw::fill_rounded_rect(shadow, ring, radius + i, Color(0, 0, 0, 12));
        }
    }
}

Rect UIWindow::title_bar_rect() const {
    Rect area = rect();
    int title_bar_height = manager->theme.misc_int("window", "title_bar_height", 28);
    return Rect(area.x, area.y, area.w, title_bar_height);
}

Rect UIWindow::close_button_rect() const {
    Rect bar = title_bar_rect();
    int size = bar.h - 10;
    return Rect(bar.x + bar.w - size - 5, bar.y + 5, size, size);
}

Rect UIWindow::client_rect() const {
    Rect area = rect();
    int border_width = manager->theme.misc_int("window", "border_width", 1);
    Rect bar = title_bar_rect();
    return Rect(area.x + border_width, area.y + bar.h, area.w - 2 * border_width,
                area.h - bar.h - border_width);
}

bool UIWindow::process_event(const Event &event) {
    Rect area = rect();
    Rect close_area = close_button_rect();

    if (event.type == EVENT_MOUSEBUTTONDOWN && event.button == SDL_BUTTON_LEFT) {
        if (close_area.collidepoint(event.x, event.y)) {
            close_held = true;
            return true;
        }
        if (area.collidepoint(event.x, event.y)) {
            return true;
        }
        return false;
    }

    if (event.type == EVENT_MOUSEBUTTONUP && event.button == SDL_BUTTON_LEFT) {
        bool was_held = close_held;
        close_held = false;
        if (was_held && close_area.collidepoint(event.x, event.y)) {
            Event closed(EVENT_UI_WINDOW_CLOSE);
            closed.ui_element = this;
            events::post(closed);

            for (std::size_t i = 0; i < manager->elements.size(); ++i) {
                if (manager->elements[i]->container == this) {
                    manager->elements[i]->kill();
                }
            }
            kill();
            return true;
        }
        if (area.collidepoint(event.x, event.y)) {
            return true;
        }
    }

    return false;
}

void UIWindow::draw(Surface &surface) {
    Rect area = rect();

    if (!shadow.empty()) {
        surface.blit(shadow, area.x - shadow_width, area.y - shadow_width);
    }

    Color background = manager->theme.colour("window", "dark_bg", Color(0x15, 0x19, 0x1e));
    Color border = manager->theme.colour("window", "normal_border", Color(0xDD, 0xDD, 0xDD));
    int radius = manager->theme.misc_int("window", "shape_corner_radius", 2);
    int border_width = manager->theme.misc_int("window", "border_width", 1);

    uidraw::rounded_rect_with_border(surface, area, radius, border_width, background, border);

    Rect bar = title_bar_rect();
    Color bar_bg = manager->theme.colour("button", "normal_bg", Color(0x45, 0x49, 0x4e));
    Color bar_border = manager->theme.colour("button", "normal_border", Color(0xDD, 0xDD, 0xDD));
    Color bar_text = manager->theme.colour("button", "normal_text", Color(0xc5, 0xcb, 0xd8));
    int bar_radius = manager->theme.misc_int("button", "shape_corner_radius", 2);
    int bar_border_width = manager->theme.misc_int("button", "border_width", 1);

    uidraw::rounded_rect_with_border(surface, bar, bar_radius, bar_border_width, bar_bg,
                                     bar_border);
    uidraw::blit_text(surface, manager->button_font(), window_display_title, bar_text, bar, "center",
                      "center");

    Rect close_area = close_button_rect();
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    Color close_bg = close_area.collidepoint(mouse_x, mouse_y)
                         ? manager->theme.colour("button", "hovered_bg", Color(0x35, 0x39, 0x3e))
                         : bar_bg;
    uidraw::rounded_rect_with_border(surface, close_area, bar_radius, bar_border_width, close_bg,
                                     bar_border);
    uidraw::blit_text(surface, manager->button_font(), "X", bar_text, close_area, "center",
                      "center");
}
