#include "ui/ui_text_box.h"

#include <cstddef>
#include <cstdlib>
#include <vector>

#include "ui/draw.h"
#include "ui/ui_manager.h"

namespace ui {

using engine::Color;
using engine::Rect;
using engine::Surface;

UiTextBox::UiTextBox(const std::string& html_text, const Rect& relative_rect,
	UiManager* manager, UiElement* container) :
	UiElement(relative_rect, manager, container), html_text_(html_text) {}

void UiTextBox::Draw(Surface& surface) {
	Rect area = AbsoluteRect();

	const Theme& theme = manager()->theme();
	Color background =
		theme.Colour("text_box", "dark_bg", Color(0x15, 0x19, 0x1e));
	Color border =
		theme.Colour("text_box", "normal_border", Color(0xDD, 0xDD, 0xDD));
	Color text_colour =
		theme.Colour("text_box", "normal_text", Color(0xc5, 0xcb, 0xd8));
	int border_width = theme.MiscInt("text_box", "border_width", 1);
	int radius = theme.MiscInt("text_box", "shape_corner_radius", 0);

	std::string padding = theme.Misc("text_box", "padding", "10,10");
	int padding_x = 10;
	int padding_y = 10;
	size_t comma = padding.find(',');
	if (comma != std::string::npos) {
		padding_x = std::atoi(padding.substr(0, comma).c_str());
		padding_y = std::atoi(padding.substr(comma + 1).c_str());
	}

	RoundedRectWithBorder(
		surface, area, radius, border_width, background, border);

	TTF_Font* font = manager()->TextBoxFont();
	int line_height = TTF_FontLineSkip(font);

	std::vector<std::string> lines;
	std::string current;
	for (size_t i = 0; i < html_text_.size(); ++i) {
		if (html_text_[i] == '\n') {
			lines.push_back(current);
			current.clear();
		} else if (html_text_[i] != '\r') {
			current += html_text_[i];
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

	for (size_t i = 0; i < lines.size(); ++i) {
		Rect line_area(area.x + padding_x,
			area.y + padding_y + static_cast<int>(i) * line_height,
			area.w - 2 * padding_x, line_height);
		BlitText(
			surface, font, lines[i], text_colour, line_area, "left", "top");
	}

	SDL_SetClipRect(surface.raw(), &previous_clip);
}

}  // namespace ui
