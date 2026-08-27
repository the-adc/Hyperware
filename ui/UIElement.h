#ifndef UI_UIELEMENT_H
#define UI_UIELEMENT_H

#include "engine/Events.h"
#include "engine/Graphics.h"

class UIManager;

class UIElement {
public:
    UIElement(const Rect &relative_rect, UIManager *manager, UIElement *container);
    virtual ~UIElement() {}

    virtual bool process_event(const Event &event) { return false; }
    virtual bool process_overlay_event(const Event &event) { return false; }
    virtual void update(double time_seconds) {}
    virtual void draw(Surface &surface) {}
    virtual void draw_overlay(Surface &surface) {}

    virtual Rect client_rect() const;
    Rect rect() const;
    void kill();

    Rect relative_rect;
    UIManager *manager;
    UIElement *container;
    int layer;
    bool alive;
};

#endif
