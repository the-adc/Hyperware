#include "ui/UIDropDownMenu.h"

#include "ui/Draw.h"
#include "ui/UIManager.h"

UIDropDownMenu::UIDropDownMenu(const std::vector<std::string> &options,
                               const std::string &selected, const Rect &relative_rect,
                               UIManager *manager, UIElement *container)
    : UIElement(relative_rect, manager, container), options(options),
      selected_option(selected, selected), expanded(false) {}

Rect UIDropDownMenu::options_rect() const {
    Rect area = rect();
    return Rect(area.x, area.y + area.h, area.w, area.h * (int)options.size());
}

bool UIDropDownMenu::process_overlay_event(const Event &event) {
    if (!expanded) {
        return false;
    }
    if (event.type != EVENT_MOUSEBUTTONDOWN || event.button != SDL_BUTTON_LEFT) {
        return false;
    }

    Rect area = rect();
    Rect list = options_rect();

    if (list.collidepoint(event.x, event.y)) {
        int index = (event.y - list.y) / area.h;
        if (index >= 0 && index < (int)options.size()) {
            selected_option = SelectedOption(options[index], options[index]);
            Event changed(EVENT_UI_DROP_DOWN_MENU_CHANGED);
            changed.ui_element = this;
            changed.selected_option = selected_option;
            events::post(changed);
        }
        expanded = false;
        return true;
    }

    expanded = false;
    if (area.collidepoint(event.x, event.y)) {
        return true;
    }
    return false;
}

bool UIDropDownMenu::process_event(const Event &event) {
    if (event.type != EVENT_MOUSEBUTTONDOWN || event.button != SDL_BUTTON_LEFT) {
        return false;
    }
    Rect area = rect();
    if (area.collidepoint(event.x, event.y)) {
        expanded = !expanded;
        return true;
    }
    return false;
}

void UIDropDownMenu::draw(Surface &surface) {
    Rect area = rect();

    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    Color normal_bg = manager->theme.colour("drop_down_menu", "normal_bg", Color(0x45, 0x49, 0x4e));
    Color hovered_bg =
        manager->theme.colour("drop_down_menu", "hovered_bg", Color(0x35, 0x39, 0x3e));
    Color border =
        manager->theme.colour("drop_down_menu", "normal_border", Color(0xDD, 0xDD, 0xDD));
    Color text_colour = manager->theme.colour("button", "normal_text", Color(0xc5, 0xcb, 0xd8));

    int radius = manager->theme.misc_int("drop_down_menu", "shape_corner_radius", 2);
    int border_width = manager->theme.misc_int("drop_down_menu", "border_width", 1);
    int open_button_width =
        manager->theme.misc_int("drop_down_menu.#selected_option", "open_button_width", 10);

    bool hovered = area.collidepoint(mouse_x, mouse_y);

    uidraw::rounded_rect_with_border(surface, area, radius, border_width,
                                     hovered ? hovered_bg : normal_bg, border);

    Rect text_area(area.x + 10, area.y, area.w - 10 - open_button_width, area.h);
    uidraw::blit_text(surface, manager->button_font(), selected_option.text, text_colour, text_area,
                      "left", "center");

    int arrow_x = area.x + area.w - open_button_width - 6;
    int arrow_y = area.y + area.h / 2 - 2;
    for (int i = 0; i < 5; ++i) {
        Rect row(arrow_x + i, arrow_y + i, 10 - 2 * i, 1);
        if (row.w <= 0) {
            break;
        }
        uidraw::fill_rounded_rect(surface, row, 0, text_colour);
    }
}

void UIDropDownMenu::draw_overlay(Surface &surface) {
    if (!expanded) {
        return;
    }

    Rect area = rect();
    Rect list = options_rect();

    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    Color normal_bg = manager->theme.colour("drop_down_menu", "normal_bg", Color(0x45, 0x49, 0x4e));
    Color hovered_bg =
        manager->theme.colour("drop_down_menu", "hovered_bg", Color(0x35, 0x39, 0x3e));
    Color border =
        manager->theme.colour("drop_down_menu", "normal_border", Color(0xDD, 0xDD, 0xDD));
    Color text_colour = manager->theme.colour("button", "normal_text", Color(0xc5, 0xcb, 0xd8));

    int list_radius =
        manager->theme.misc_int("drop_down_menu.#drop_down_options_list", "shape_corner_radius", 5);
    int list_border =
        manager->theme.misc_int("drop_down_menu.#drop_down_options_list", "border_width", 1);

    uidraw::rounded_rect_with_border(surface, list, list_radius, list_border, normal_bg, border);

    for (std::size_t i = 0; i < options.size(); ++i) {
        Rect row(list.x + list_border, list.y + list_border + (int)i * area.h,
                 list.w - 2 * list_border, area.h);
        bool hovered = row.collidepoint(mouse_x, mouse_y);
        if (hovered) {
            uidraw::fill_rounded_rect(surface, row, list_radius, hovered_bg);
        }
        Rect text_area(row.x + 10, row.y, row.w - 10, row.h);
        uidraw::blit_text(surface, manager->button_font(), options[i], text_colour, text_area,
                          "left", "center");
    }
}
