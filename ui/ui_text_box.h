#ifndef HYPER_UI_UI_TEXT_BOX_H_
#define HYPER_UI_UI_TEXT_BOX_H_

#include <string>

#include "ui/ui_element.h"

namespace ui {

class UiTextBox : public UiElement {
public:
	UiTextBox(const std::string& html_text, const engine::Rect& relative_rect,
		UiManager* manager, UiElement* container = nullptr);

	void Draw(engine::Surface& surface) override;

	const std::string& html_text() const { return html_text_; }
	void set_html_text(const std::string& html_text) { html_text_ = html_text; }

private:
	std::string html_text_;
};

}  // namespace ui

#endif  // HYPER_UI_UI_TEXT_BOX_H_
