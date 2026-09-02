#include "ui/ui_manager.h"

#include <cstddef>

namespace ui {

UiManager::UiManager(const engine::Vec2& size, const std::string& theme_path) :
	size_(size) {
	theme_.Load(theme_path);
}

UiManager::~UiManager() {
	for (size_t i = 0; i < elements_.size(); ++i) {
		delete elements_[i];
	}
	elements_.clear();
}

void UiManager::RegisterElement(UiElement* element) {
	elements_.push_back(element);
}

TTF_Font* UiManager::ButtonFont() {
	std::string path = theme_.FontPath("button", "assets/fonts/CALIST.ttf");
	int size_points = theme_.FontSize("button", 18);
	return engine::graphics::LoadFont(path, size_points);
}

TTF_Font* UiManager::TextBoxFont() {
	std::string path = theme_.FontPath(
		"text_box", theme_.FontPath("button", "assets/fonts/CALIST.ttf"));
	int size_points = theme_.FontSize("text_box", 14);
	return engine::graphics::LoadFont(path, size_points);
}

void UiManager::ProcessEvents(const engine::Event& event) {
	for (size_t i = elements_.size(); i-- > 0;) {
		if (!elements_[i]->alive()) {
			continue;
		}
		if (elements_[i]->ProcessOverlayEvent(event)) {
			return;
		}
	}

	for (int layer = 1; layer >= 0; --layer) {
		for (size_t i = elements_.size(); i-- > 0;) {
			if (!elements_[i]->alive() || elements_[i]->layer() != layer) {
				continue;
			}
			if (elements_[i]->ProcessEvent(event)) {
				return;
			}
		}
	}
}

void UiManager::Update(double time_seconds) {
	for (size_t i = 0; i < elements_.size(); ++i) {
		if (elements_[i]->alive()) {
			elements_[i]->Update(time_seconds);
		}
	}
}

void UiManager::DrawUi(engine::Surface& surface) {
	for (int layer = 0; layer <= 1; ++layer) {
		for (size_t i = 0; i < elements_.size(); ++i) {
			if (elements_[i]->alive() && elements_[i]->layer() == layer) {
				elements_[i]->Draw(surface);
			}
		}
	}
	for (size_t i = 0; i < elements_.size(); ++i) {
		if (elements_[i]->alive()) {
			elements_[i]->DrawOverlay(surface);
		}
	}
}

}
