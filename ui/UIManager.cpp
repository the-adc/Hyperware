#include "ui/UIManager.h"

#include <algorithm>

UIElement::UIElement(const Rect &relative_rect, UIManager *manager, UIElement *container)
    : relative_rect(relative_rect), manager(manager), container(container), layer(0), alive(true) {
    if (container) {
        layer = container->layer;
    }
    if (manager) {
        manager->register_element(this);
    }
}

Rect UIElement::client_rect() const {
    return rect();
}

Rect UIElement::rect() const {
    if (container) {
        Rect base = container->client_rect();
        return Rect(base.x + relative_rect.x, base.y + relative_rect.y, relative_rect.w,
                    relative_rect.h);
    }
    return relative_rect;
}

void UIElement::kill() {
    alive = false;
}

UIManager::UIManager(const Vec2 &size, const std::string &theme_path) : size(size) {
    theme.load(theme_path);
}

UIManager::~UIManager() {
    for (std::size_t i = 0; i < elements.size(); ++i) {
        delete elements[i];
    }
    elements.clear();
}

void UIManager::register_element(UIElement *element) {
    elements.push_back(element);
}

TTF_Font *UIManager::button_font() {
    std::string path = theme.font_path("button", "assets/fonts/CALIST.ttf");
    int size_points = theme.font_size("button", 18);
    return graphics::load_font(path, size_points);
}

TTF_Font *UIManager::text_box_font() {
    std::string path = theme.font_path("text_box", theme.font_path("button", "assets/fonts/CALIST.ttf"));
    int size_points = theme.font_size("text_box", 14);
    return graphics::load_font(path, size_points);
}

void UIManager::process_events(const Event &event) {
    for (std::size_t i = elements.size(); i-- > 0;) {
        if (!elements[i]->alive) {
            continue;
        }
        if (elements[i]->process_overlay_event(event)) {
            return;
        }
    }

    for (int layer = 1; layer >= 0; --layer) {
        for (std::size_t i = elements.size(); i-- > 0;) {
            if (!elements[i]->alive || elements[i]->layer != layer) {
                continue;
            }
            if (elements[i]->process_event(event)) {
                return;
            }
        }
    }
}

void UIManager::update(double time_seconds) {
    for (std::size_t i = 0; i < elements.size(); ++i) {
        if (elements[i]->alive) {
            elements[i]->update(time_seconds);
        }
    }
}

void UIManager::draw_ui(Surface &surface) {
    for (int layer = 0; layer <= 1; ++layer) {
        for (std::size_t i = 0; i < elements.size(); ++i) {
            if (elements[i]->alive && elements[i]->layer == layer) {
                elements[i]->draw(surface);
            }
        }
    }
    for (std::size_t i = 0; i < elements.size(); ++i) {
        if (elements[i]->alive) {
            elements[i]->draw_overlay(surface);
        }
    }
}
