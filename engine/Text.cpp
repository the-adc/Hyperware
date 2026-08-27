#include "engine/Text.h"

#include <stdexcept>

Text::Text(const std::string &text_value, const Vec2 &pos, const Color &color,
           const std::string &font_name, const std::string &alignment) {
    font = graphics::load_font("assets/fonts/" + font_name + ".ttf", 36);
    this->pos = pos;
    this->color = color;
    text = graphics::render_text(font, text_value, this->color);
    this->alignment = alignment;
    set_alignment(this->alignment);
}

void Text::draw(Surface &surface) {
    surface.blit(text, text_rect.x, text_rect.y);
}

void Text::set_text(const std::string &text_value) {
    text = graphics::render_text(font, text_value, color);
    set_alignment(alignment);
}

void Text::set_alignment(const std::string &alignment_value) {
    if (alignment_value != "center" && alignment_value != "topleft" &&
        alignment_value != "topright" && alignment_value != "bottomleft" &&
        alignment_value != "bottomright") {
        throw std::invalid_argument("Invalid alignment. Choose from 'center', 'topleft', "
                                    "'topright', 'bottomleft', or 'bottomright'.");
    }
    alignment = alignment_value;
    text_rect = text.get_rect();

    int anchor_x = graphics::trunc(pos.x);
    int anchor_y = graphics::trunc(pos.y);

    if (alignment == "center") {
        text_rect.set_center(anchor_x, anchor_y);
    } else if (alignment == "topleft") {
        text_rect.x = anchor_x;
        text_rect.y = anchor_y;
    } else if (alignment == "topright") {
        text_rect.x = anchor_x - text_rect.w;
        text_rect.y = anchor_y;
    } else if (alignment == "bottomleft") {
        text_rect.x = anchor_x;
        text_rect.y = anchor_y - text_rect.h;
    } else {
        text_rect.x = anchor_x - text_rect.w;
        text_rect.y = anchor_y - text_rect.h;
    }
}
