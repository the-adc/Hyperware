#include "engine/Sprite.h"

Sprite::Sprite(double x, double y, const std::string &image, Vec2 scale, double angle)
    : pos(x, y), scale(scale), angle(angle) {
    size[0] = 0;
    size[1] = 0;
    set_sprite(image);
}

void Sprite::draw(Surface &surface) {
    surface.blit(image, rect.x, rect.y);
}

void Sprite::set_sprite(const std::string &image_name) {
    filename = image_name;
    const Surface &sprite = graphics::load_image("assets/sprites/" + image_name + ".png");

    size[0] = sprite.get_width();
    size[1] = sprite.get_height();

    image = graphics::scale(sprite, (int)(size[0] * scale[0]), (int)(size[1] * scale[1]));

    image = graphics::rotate(image, angle);
    rect = image.get_rect();
    rect.set_center(pos);
}

void Sprite::set_scale(double xscale, double yscale) {
    scale = Vec2(xscale, yscale);

    const Surface &sprite = graphics::load_image("assets/sprites/" + filename + ".png");
    image = graphics::scale(sprite, (int)(size[0] * scale[0]), (int)(size[1] * scale[1]));
    rect = image.get_rect();
    rect.set_center(pos);
}

void Sprite::set_position(const Vec2 &position) {
    pos = position;
    rect.set_center(pos);
}

void Sprite::set_position_smooth(const Vec2 &position, double amount) {
    pos = Vec2::slerp(pos, position, amount);

    set_position(pos);
}
