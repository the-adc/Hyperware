#include "ui/ui_drop_down_menu.h"

#include <cstddef>

#include "ui/draw.h"
#include "ui/ui_manager.h"

namespace ui {

using engine::Color;
using engine::Event;
using engine::EventType;
using engine::Rect;
using engine::SelectedOption;
using engine::Surface;

UiDropDownMenu::UiDropDownMenu(const std::vector<std::string>& options,
	const std::string& selected, const Rect& relative_rect, UiManager* manager,
	UiElement* container) :
	UiElement(relative_rect, manager, container),
	options_(options),
	selected_option_(selected, selected),
	expanded_(false) {}

Rect UiDropDownMenu::OptionsRect() const {
	Rect area = AbsoluteRect();
	return Rect(area.x, area.y + area.h, area.w,
		area.h * static_cast<int>(options_.size()));
}

bool UiDropDownMenu::ProcessOverlayEvent(const Event& event) {
	if (!expanded_) {
		return false;
	}
	if (event.type != EventType::kMouseButtonDown ||
		event.button != SDL_BUTTON_LEFT) {
		return false;
	}

	Rect area = AbsoluteRect();
	Rect list = OptionsRect();

	if (list.CollidePoint(event.x, event.y)) {
		int index = (event.y - list.y) / area.h;
		if (index >= 0 && index < static_cast<int>(options_.size())) {
			selected_option_ = SelectedOption(options_[index], options_[index]);
			Event changed(EventType::kUiDropDownMenuChanged);
			changed.ui_element = this;
			changed.selected_option = selected_option_;
			engine::events::Post(changed);
		}
		expanded_ = false;
		return true;
	}

	expanded_ = false;
	if (area.CollidePoint(event.x, event.y)) {
		return true;
	}
	return false;
}

bool UiDropDownMenu::ProcessEvent(const Event& event) {
	if (event.type != EventType::kMouseButtonDown ||
		event.button != SDL_BUTTON_LEFT) {
		return false;
	}
	Rect area = AbsoluteRect();
	if (area.CollidePoint(event.x, event.y)) {
		expanded_ = !expanded_;
		return true;
	}
	return false;
}

void UiDropDownMenu::Draw(Surface& surface) {
	Rect area = AbsoluteRect();

	int mouse_x = 0;
	int mouse_y = 0;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	const Theme& theme = manager()->theme();
	Color normal_bg =
		theme.Colour("drop_down_menu", "normal_bg", Color(0x45, 0x49, 0x4e));
	Color hovered_bg =
		theme.Colour("drop_down_menu", "hovered_bg", Color(0x35, 0x39, 0x3e));
	Color border = theme.Colour(
		"drop_down_menu", "normal_border", Color(0xDD, 0xDD, 0xDD));
	Color text_colour =
		theme.Colour("button", "normal_text", Color(0xc5, 0xcb, 0xd8));

	int radius = theme.MiscInt("drop_down_menu", "shape_corner_radius", 2);
	int border_width = theme.MiscInt("drop_down_menu", "border_width", 1);
	int open_button_width = theme.MiscInt(
		"drop_down_menu.#selected_option", "open_button_width", 10);

	bool hovered = area.CollidePoint(mouse_x, mouse_y);

	RoundedRectWithBorder(surface, area, radius, border_width,
		hovered ? hovered_bg : normal_bg, border);

	Rect text_area(
		area.x + 10, area.y, area.w - 10 - open_button_width, area.h);
	BlitText(surface, manager()->ButtonFont(), selected_option_.text,
		text_colour, text_area, "left", "center");

	int arrow_x = area.x + area.w - open_button_width - 6;
	int arrow_y = area.y + area.h / 2 - 2;
	for (int i = 0; i < 5; ++i) {
		Rect row(arrow_x + i, arrow_y + i, 10 - 2 * i, 1);
		if (row.w <= 0) {
			break;
		}
		FillRoundedRect(surface, row, 0, text_colour);
	}
}

void UiDropDownMenu::DrawOverlay(Surface& surface) {
	if (!expanded_) {
		return;
	}

	Rect area = AbsoluteRect();
	Rect list = OptionsRect();

	int mouse_x = 0;
	int mouse_y = 0;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	const Theme& theme = manager()->theme();
	Color normal_bg =
		theme.Colour("drop_down_menu", "normal_bg", Color(0x45, 0x49, 0x4e));
	Color hovered_bg =
		theme.Colour("drop_down_menu", "hovered_bg", Color(0x35, 0x39, 0x3e));
	Color border = theme.Colour(
		"drop_down_menu", "normal_border", Color(0xDD, 0xDD, 0xDD));
	Color text_colour =
		theme.Colour("button", "normal_text", Color(0xc5, 0xcb, 0xd8));

	int list_radius = theme.MiscInt(
		"drop_down_menu.#drop_down_options_list", "shape_corner_radius", 5);
	int list_border = theme.MiscInt(
		"drop_down_menu.#drop_down_options_list", "border_width", 1);

	RoundedRectWithBorder(
		surface, list, list_radius, list_border, normal_bg, border);

	for (size_t i = 0; i < options_.size(); ++i) {
		Rect row(list.x + list_border,
			list.y + list_border + static_cast<int>(i) * area.h,
			list.w - 2 * list_border, area.h);
		bool hovered = row.CollidePoint(mouse_x, mouse_y);
		if (hovered) {
			FillRoundedRect(surface, row, list_radius, hovered_bg);
		}
		Rect text_area(row.x + 10, row.y, row.w - 10, row.h);
		BlitText(surface, manager()->ButtonFont(), options_[i], text_colour,
			text_area, "left", "center");
	}
}

}  // namespace ui
