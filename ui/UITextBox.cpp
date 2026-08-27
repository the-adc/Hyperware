#include "ui/UITextBox.h"

#include <cstdlib>
#include <vector>

#include "ui/Draw.h"
#include "ui/UIManager.h"

UITextBox::UITextBox(const std::string &html_text, const Rect &relative_rect, UIManager *manager,
                     UIElement *container)
    : UIElement(relative_rect, manager, container), html_text(html_text) {}

void UITextBox::draw(Surface &surface) {
    Rect area = rect();

    Color background = manager->theme.colour("text_box", "dark_bg", Color(0x15, 0x19, 0x1e));
    Color border = manager->theme.colour("text_box", "normal_border", Color(0xDD, 0xDD, 0xDD));
    Color text_colour = manager->theme.colour("text_box", "normal_text", Color(0xc5, 0xcb, 0xd8));
    int border_width = manager->theme.misc_int("text_box", "border_width", 1);
    int radius = manager->theme.misc_int("text_box", "shape_corner_radius", 0);

    std::string padding = manager->theme.misc("text_box", "padding", "10,10");
    int padding_x = 10;
    int padding_y = 10;
    std::size_t comma = padding.find(',');
    if (comma != std::string::npos) {
        padding_x = std::atoi(padding.substr(0, comma).c_str());
        padding_y = std::atoi(padding.substr(comma + 1).c_str());
    }

    uidraw::rounded_rect_with_border(surface, area, radius, border_width, background, border);

    TTF_Font *font = manager->text_box_font();
    int line_height = TTF_FontLineSkip(font);

    std::vector<std::string> lines;
    std::string current;
    for (std::size_t i = 0; i < html_text.size(); ++i) {
        if (html_text[i] == '\n') {
            lines.push_back(current);
            current.clear();
        } else if (html_text[i] != '\r') {
            current += html_text[i];
        }
    }
    lines.push_back(current);

    SDL_Rect previous_clip;
    SDL_GetClipRect(surface.raw(), &previous_clip);
    SDL_Rect clip;
    clip.x = area.x + border_width;
    clip.y = area.y + border_width;
    clip.w = area.w - 2 * border_width;
    clip.h = area.h - 2 * border_width;
    SDL_SetClipRect(surface.raw(), &clip);

    for (std::size_t i = 0; i < lines.size(); ++i) {
        Rect line_area(area.x + padding_x, area.y + padding_y + (int)i * line_height,
                       area.w - 2 * padding_x, line_height);
        uidraw::blit_text(surface, font, lines[i], text_colour, line_area, "left", "top");
    }

    SDL_SetClipRect(surface.raw(), &previous_clip);
}
