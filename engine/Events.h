#ifndef ENGINE_EVENTS_H
#define ENGINE_EVENTS_H

#include <SDL.h>

#include <string>
#include <vector>

enum EventType {
    EVENT_NONE = 0,
    EVENT_QUIT,
    EVENT_KEYDOWN,
    EVENT_KEYUP,
    EVENT_MOUSEBUTTONDOWN,
    EVENT_MOUSEBUTTONUP,
    EVENT_MOUSEMOTION,
    EVENT_UI_BUTTON_PRESSED,
    EVENT_UI_DROP_DOWN_MENU_CHANGED,
    EVENT_UI_WINDOW_CLOSE
};

struct SelectedOption {
    std::string text;
    std::string id;

    SelectedOption() {}
    SelectedOption(const std::string &text, const std::string &id) : text(text), id(id) {}

    const std::string &operator[](int index) const { return index == 0 ? text : id; }
};

struct Event {
    int type;
    int key;
    int button;
    int x;
    int y;
    void *ui_element;
    SelectedOption selected_option;

    Event() : type(EVENT_NONE), key(0), button(0), x(0), y(0), ui_element(nullptr) {}
    explicit Event(int type) : type(type), key(0), button(0), x(0), y(0), ui_element(nullptr) {}
};

namespace events {

void post(const Event &event);
std::vector<Event> get();

}

#endif
