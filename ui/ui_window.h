#ifndef HYPER_UI_UI_WINDOW_H_
#define HYPER_UI_UI_WINDOW_H_

#include <string>

#include "ui/ui_element.h"

namespace ui {

class UiWindow : public UiElement {
public:
	UiWindow(const engine::Rect& window_rect, UiManager* manager,
		const std::string& window_display_title, bool resizable = false,
		bool draggable = false);

	bool ProcessEvent(const engine::Event& event) override;
	void Draw(engine::Surface& surface) override;
	engine::Rect ClientRect() const override;

	engine::Rect TitleBarRect() const;
	engine::Rect CloseButtonRect() const;

	const std::string& window_display_title() const {
		return window_display_title_;
	}
	bool resizable() const { return resizable_; }
	bool draggable() const { return draggable_; }
	bool close_held() const { return close_held_; }
	int shadow_width() const { return shadow_width_; }

private:
	std::string window_display_title_;
	bool resizable_;
	bool draggable_;
	bool close_held_;
	engine::Surface shadow_;
	int shadow_width_;
};

}  // namespace ui

#endif  // HYPER_UI_UI_WINDOW_H_
