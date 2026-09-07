#include "ui/ui_button.h"

#include "ui/draw.h"
#include "ui/ui_manager.h"

namespace ui {

using engine::Color;
using engine::Event;
using engine::EventType;
using engine::Rect;
using engine::Surface;

UiButton::UiButton(const Rect& relative_rect, const std::string& text,
	UiManager* manager, UiElement* container, const std::string& theme_block) :
	UiElement(relative_rect, manager, container),
	text_(text),
	theme_block_(theme_block),
	held_(false) {}

bool UiButton::ProcessEvent(const Event& event) {
	Rect area = AbsoluteRect();

	if (event.type == EventType::kMouseButtonDown &&
		event.button == SDL_BUTTON_LEFT) {
		if (area.CollidePoint(event.x, event.y)) {
			held_ = true;
			return true;
		}
		return false;
	}

	if (event.type == EventType::kMouseButtonUp &&
		event.button == SDL_BUTTON_LEFT) {
		bool was_held = held_;
		held_ = false;
		if (was_held && area.CollidePoint(event.x, event.y)) {
			Event pressed(EventType::kUiButtonPressed);
			pressed.ui_element = this;
			engine::events::Post(pressed);
			return true;
		}
	}

	return false;
}

void UiButton::Draw(Surface& surface) {
	Rect area = AbsoluteRect();

	int mouse_x = 0;
	int mouse_y = 0;
	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
	bool hovered = area.CollidePoint(mouse_x, mouse_y);
	bool pressed = held_ && hovered && (buttons & SDL_BUTTON(SDL_BUTTON_LEFT));

	const Theme& theme = manager()->theme();
	Color normal_bg =
		theme.Colour(theme_block_, "normal_bg", Color(0x45, 0x49, 0x4e));
	Color hovered_bg =
		theme.Colour(theme_block_, "hovered_bg", Color(0x35, 0x39, 0x3e));
	Color active_bg =
		theme.Colour(theme_block_, "active_bg", Color(0x19, 0x37, 0x54));
	Color border =
		theme.Colour(theme_block_, "normal_border", Color(0xDD, 0xDD, 0xDD));
	Color normal_text =
		theme.Colour(theme_block_, "normal_text", Color(0xc5, 0xcb, 0xd8));
	Color hovered_text =
		theme.Colour(theme_block_, "hovered_text", Color(255, 255, 255));

	Color background = normal_bg;
	Color foreground = normal_text;
	if (pressed) {
		background = active_bg;
		foreground = hovered_text;
	} else if (hovered) {
		background = hovered_bg;
		foreground = hovered_text;
	}

	int radius = theme.MiscInt(theme_block_, "shape_corner_radius", 2);
	int border_width = theme.MiscInt(theme_block_, "border_width", 1);

	RoundedRectWithBorder(
		surface, area, radius, border_width, background, border);
	BlitText(surface, manager()->ButtonFont(), text_, foreground, area,
		"center", "center");
}

}  // namespace ui
