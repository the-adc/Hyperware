#ifndef UI_UI_BUTTON_H_
#define UI_UI_BUTTON_H_

#include <string>

#include "ui/ui_element.h"

namespace ui {

class UiButton : public UiElement {
public:
	UiButton(const engine::Rect& relative_rect, const std::string& text,
		UiManager* manager, UiElement* container = nullptr,
		const std::string& theme_block = "button");

	bool ProcessEvent(const engine::Event& event) override;
	void Draw(engine::Surface& surface) override;

	const std::string& text() const { return text_; }
	void set_text(const std::string& text) { text_ = text; }
	const std::string& theme_block() const { return theme_block_; }
	bool held() const { return held_; }

private:
	std::string text_;
	std::string theme_block_;
	bool held_;
};

}

#endif
