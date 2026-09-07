#ifndef HYPER_UI_UI_DROP_DOWN_MENU_H_
#define HYPER_UI_UI_DROP_DOWN_MENU_H_

#include <string>
#include <vector>

#include "engine/events.h"
#include "ui/ui_element.h"

namespace ui {

class UiDropDownMenu : public UiElement {
public:
	UiDropDownMenu(const std::vector<std::string>& options,
		const std::string& selected, const engine::Rect& relative_rect,
		UiManager* manager, UiElement* container = nullptr);

	bool ProcessEvent(const engine::Event& event) override;
	bool ProcessOverlayEvent(const engine::Event& event) override;
	void Draw(engine::Surface& surface) override;
	void DrawOverlay(engine::Surface& surface) override;

	engine::Rect OptionsRect() const;

	const std::vector<std::string>& options() const { return options_; }
	const engine::SelectedOption& selected_option() const {
		return selected_option_;
	}
	bool expanded() const { return expanded_; }

private:
	std::vector<std::string> options_;
	engine::SelectedOption selected_option_;
	bool expanded_;
};

}  // namespace ui

#endif  // HYPER_UI_UI_DROP_DOWN_MENU_H_
