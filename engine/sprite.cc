#include "engine/sprite.h"

#include <optional>

namespace engine {

Sprite::Sprite(double x, double y, const std::string& image, const Vec2& scale,
	double angle) :
	size_{0, 0}, pos_(x, y), scale_(scale), angle_(angle) {
	SetSprite(image);
}

void Sprite::Draw(Surface& surface) { surface.Blit(image_, rect_.x, rect_.y); }

void Sprite::SetSprite(const std::string& image) {
	filename_ = image;
	const Surface& sprite =
		graphics::LoadImageFile("assets/sprites/" + image + ".png");

	size_[0] = sprite.width();
	size_[1] = sprite.height();

	image_ = graphics::Scale(sprite, static_cast<int>(size_[0] * scale_[0]),
		static_cast<int>(size_[1] * scale_[1]));

	image_ = graphics::Rotate(image_, angle_);
	rect_ = image_.rect();
	rect_.SetCenter(pos_);
}

void Sprite::SetScale(double xscale, double yscale) {
	scale_ = Vec2(xscale, yscale);

	const Surface& sprite =
		graphics::LoadImageFile("assets/sprites/" + filename_ + ".png");
	image_ = graphics::Scale(sprite, static_cast<int>(size_[0] * scale_[0]),
		static_cast<int>(size_[1] * scale_[1]));
	rect_ = image_.rect();
	rect_.SetCenter(pos_);
}

void Sprite::SetPosition(const Vec2& pos) {
	pos_ = pos;
	rect_.SetCenter(pos_);
}

void Sprite::SetPositionSmooth(const Vec2& pos, double amount) {
	std::optional<Vec2> next = Vec2::Slerp(pos_, pos, amount);
	if (next) {
		SetPosition(*next);
	}
}

}
