#ifndef UI_UIMANAGER_H
#define UI_UIMANAGER_H

#include <string>
#include <vector>

#include "engine/Events.h"
#include "engine/Graphics.h"
#include "engine/Vec2.h"
#include "ui/Theme.h"
#include "ui/UIElement.h"

class UIManager {
public:
    UIManager(const Vec2 &size, const std::string &theme_path);
    ~UIManager();

    void register_element(UIElement *element);

    void process_events(const Event &event);
    void update(double time_seconds);
    void draw_ui(Surface &surface);

    TTF_Font *button_font();
    TTF_Font *text_box_font();

    Theme theme;
    Vec2 size;
    std::vector<UIElement *> elements;
};

#endif
