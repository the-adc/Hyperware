#include "engine/text.h"

namespace engine {

Text::Text(const std::string& text, const Vec2& pos, const Color& color,
	const std::string& font, Alignment alignment) :
	font_(graphics::LoadFont("assets/fonts/" + font + ".ttf", 36)),
	pos_(pos),
	color_(color),
	alignment_(alignment) {
	text_ = graphics::RenderText(font_, text, color_);
	SetAlignment(alignment_);
}

void Text::Draw(Surface& surface) {
	surface.Blit(text_, text_rect_.x, text_rect_.y);
}

void Text::SetText(const std::string& text) {
	text_ = graphics::RenderText(font_, text, color_);
	SetAlignment(alignment_);
}

void Text::SetPosition(const Vec2& pos) {
	pos_ = pos;
	SetAlignment(alignment_);
}

void Text::SetAlignment(Alignment alignment) {
	alignment_ = alignment;
	text_rect_ = text_.rect();

	int anchor_x = graphics::Trunc(pos_.x);
	int anchor_y = graphics::Trunc(pos_.y);

	switch (alignment_) {
		case Alignment::kCenter:
			text_rect_.SetCenter(anchor_x, anchor_y);
			break;
		case Alignment::kTopLeft:
			text_rect_.x = anchor_x;
			text_rect_.y = anchor_y;
			break;
		case Alignment::kTopRight:
			text_rect_.x = anchor_x - text_rect_.w;
			text_rect_.y = anchor_y;
			break;
		case Alignment::kBottomLeft:
			text_rect_.x = anchor_x;
			text_rect_.y = anchor_y - text_rect_.h;
			break;
		case Alignment::kBottomRight:
			text_rect_.x = anchor_x - text_rect_.w;
			text_rect_.y = anchor_y - text_rect_.h;
			break;
	}
}

}  // namespace engine
