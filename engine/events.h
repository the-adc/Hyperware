#ifndef HYPER_ENGINE_EVENTS_H_
#define HYPER_ENGINE_EVENTS_H_

#include <SDL.h>

#include <string>
#include <vector>

namespace engine {

enum class EventType {
	kNone = 0,
	kQuit,
	kKeyDown,
	kKeyUp,
	kMouseButtonDown,
	kMouseButtonUp,
	kMouseMotion,
	kUiButtonPressed,
	kUiDropDownMenuChanged,
	kUiWindowClose
};

struct SelectedOption {
	std::string text;
	std::string id;

	SelectedOption() = default;
	SelectedOption(const std::string& text, const std::string& id) :
		text(text), id(id) {}

	const std::string& operator[](int index) const {
		return index == 0 ? text : id;
	}
};

struct Event {
	EventType type = EventType::kNone;
	int key = 0;
	int button = 0;
	int x = 0;
	int y = 0;
	void* ui_element = nullptr;
	SelectedOption selected_option;

	Event() = default;
	explicit Event(EventType type) : type(type) {}
};

namespace events {

void Post(const Event& event);
std::vector<Event> Get();

}  // namespace events

}  // namespace engine

#endif  // HYPER_ENGINE_EVENTS_H_
