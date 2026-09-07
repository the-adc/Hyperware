#ifndef HYPER_UI_DRAW_H_
#define HYPER_UI_DRAW_H_

#include <string>

#include "engine/graphics.h"

namespace ui {

void FillRoundedRect(engine::Surface& dst, const engine::Rect& area, int radius,
	const engine::Color& color);
void RoundedRectWithBorder(engine::Surface& dst, const engine::Rect& area,
	int radius, int border_width, const engine::Color& background,
	const engine::Color& border);
void BlitText(engine::Surface& dst, TTF_Font* font, const std::string& text,
	const engine::Color& color, const engine::Rect& area,
	const std::string& horizontal, const std::string& vertical);

}  // namespace ui

#endif  // HYPER_UI_DRAW_H_
