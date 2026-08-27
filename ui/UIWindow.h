#ifndef UI_UIWINDOW_H
#define UI_UIWINDOW_H

#include <string>

#include "ui/UIElement.h"

class UIWindow : public UIElement {
public:
    UIWindow(const Rect &window_rect, UIManager *manager, const std::string &window_display_title,
             bool resizable = false, bool draggable = false);

    bool process_event(const Event &event) override;
    void draw(Surface &surface) override;
    Rect client_rect() const override;

    Rect title_bar_rect() const;
    Rect close_button_rect() const;

    std::string window_display_title;
    bool resizable;
    bool draggable;
    bool close_held;
    Surface shadow;
    int shadow_width;
};

#endif
