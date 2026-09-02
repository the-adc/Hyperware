#include "ui/ui_window.h"

#include <cstddef>

#include "ui/draw.h"
#include "ui/ui_manager.h"

namespace ui {

using engine::Color;
using engine::Event;
using engine::EventType;
using engine::Rect;
using engine::Surface;

UiWindow::UiWindow(const Rect& window_rect, UiManager* manager,
	const std::string& window_display_title, bool resizable, bool draggable) :
	UiElement(window_rect, manager, nullptr),
	window_display_title_(window_display_title),
	resizable_(resizable),
	draggable_(draggable),
	close_held_(false) {
	set_layer(1);

	shadow_width_ = manager->theme().MiscInt("window", "shadow_width", 15);
	int radius = manager->theme().MiscInt("window", "shape_corner_radius", 2);

	shadow_ = Surface::Create(
		window_rect.w + 2 * shadow_width_, window_rect.h + 2 * shadow_width_);
	if (!shadow_.empty()) {
		shadow_.Fill(Color(0, 0, 0, 0));
		for (int i = shadow_width_; i >= 1; --i) {
			Rect ring(shadow_width_ - i, shadow_width_ - i,
				window_rect.w + 2 * i, window_rect.h + 2 * i);
			FillRoundedRect(shadow_, ring, radius + i, Color(0, 0, 0, 12));
		}
	}
}

Rect UiWindow::TitleBarRect() const {
	Rect area = AbsoluteRect();
	int title_bar_height =
		manager()->theme().MiscInt("window", "title_bar_height", 28);
	return Rect(area.x, area.y, area.w, title_bar_height);
}

Rect UiWindow::CloseButtonRect() const {
	Rect bar = TitleBarRect();
	int size = bar.h - 10;
	return Rect(bar.x + bar.w - size - 5, bar.y + 5, size, size);
}

Rect UiWindow::ClientRect() const {
	Rect area = AbsoluteRect();
	int border_width = manager()->theme().MiscInt("window", "border_width", 1);
	Rect bar = TitleBarRect();
	return Rect(area.x + border_width, area.y + bar.h,
		area.w - 2 * border_width, area.h - bar.h - border_width);
}

bool UiWindow::ProcessEvent(const Event& event) {
	Rect area = AbsoluteRect();
	Rect close_area = CloseButtonRect();

	if (event.type == EventType::kMouseButtonDown &&
		event.button == SDL_BUTTON_LEFT) {
		if (close_area.CollidePoint(event.x, event.y)) {
			close_held_ = true;
			return true;
		}
		if (area.CollidePoint(event.x, event.y)) {
			return true;
		}
		return false;
	}

	if (event.type == EventType::kMouseButtonUp &&
		event.button == SDL_BUTTON_LEFT) {
		bool was_held = close_held_;
		close_held_ = false;
		if (was_held && close_area.CollidePoint(event.x, event.y)) {
			Event closed(EventType::kUiWindowClose);
			closed.ui_element = this;
			engine::events::Post(closed);

			for (size_t i = 0; i < manager()->elements().size(); ++i) {
				if (manager()->elements()[i]->container() == this) {
					manager()->elements()[i]->Kill();
				}
			}
			Kill();
			return true;
		}
		if (area.CollidePoint(event.x, event.y)) {
			return true;
		}
	}

	return false;
}

void UiWindow::Draw(Surface& surface) {
	Rect area = AbsoluteRect();

	if (!shadow_.empty()) {
		surface.Blit(shadow_, area.x - shadow_width_, area.y - shadow_width_);
	}

	const Theme& theme = manager()->theme();
	Color background =
		theme.Colour("window", "dark_bg", Color(0x15, 0x19, 0x1e));
	Color border =
		theme.Colour("window", "normal_border", Color(0xDD, 0xDD, 0xDD));
	int radius = theme.MiscInt("window", "shape_corner_radius", 2);
	int border_width = theme.MiscInt("window", "border_width", 1);

	RoundedRectWithBorder(
		surface, area, radius, border_width, background, border);

	Rect bar = TitleBarRect();
	Color bar_bg = theme.Colour("button", "normal_bg", Color(0x45, 0x49, 0x4e));
	Color bar_border =
		theme.Colour("button", "normal_border", Color(0xDD, 0xDD, 0xDD));
	Color bar_text =
		theme.Colour("button", "normal_text", Color(0xc5, 0xcb, 0xd8));
	int bar_radius = theme.MiscInt("button", "shape_corner_radius", 2);
	int bar_border_width = theme.MiscInt("button", "border_width", 1);

	RoundedRectWithBorder(
		surface, bar, bar_radius, bar_border_width, bar_bg, bar_border);
	BlitText(surface, manager()->ButtonFont(), window_display_title_, bar_text,
		bar, "center", "center");

	Rect close_area = CloseButtonRect();
	int mouse_x = 0;
	int mouse_y = 0;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	Color close_bg = close_area.CollidePoint(mouse_x, mouse_y)
		? theme.Colour("button", "hovered_bg", Color(0x35, 0x39, 0x3e))
		: bar_bg;
	RoundedRectWithBorder(surface, close_area, bar_radius, bar_border_width,
		close_bg, bar_border);
	BlitText(surface, manager()->ButtonFont(), "X", bar_text, close_area,
		"center", "center");
}

}
