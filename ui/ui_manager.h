#ifndef HYPER_UI_UI_MANAGER_H_
#define HYPER_UI_UI_MANAGER_H_

#include <string>
#include <vector>

#include "engine/events.h"
#include "engine/graphics.h"
#include "engine/vec2.h"
#include "ui/theme.h"
#include "ui/ui_element.h"

namespace ui {

class UiManager {
public:
	UiManager(const engine::Vec2& size, const std::string& theme_path);

	UiManager(const UiManager&) = delete;
	UiManager& operator=(const UiManager&) = delete;

	~UiManager();

	void RegisterElement(UiElement* element);

	void ProcessEvents(const engine::Event& event);
	void Update(double time_seconds);
	void DrawUi(engine::Surface& surface);

	TTF_Font* ButtonFont();
	TTF_Font* TextBoxFont();

	const Theme& theme() const { return theme_; }
	const engine::Vec2& size() const { return size_; }
	const std::vector<UiElement*>& elements() const { return elements_; }

private:
	Theme theme_;
	engine::Vec2 size_;
	std::vector<UiElement*> elements_;
};

}  // namespace ui

#endif  // HYPER_UI_UI_MANAGER_H_
