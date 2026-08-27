#ifndef UI_UIBUTTON_H
#define UI_UIBUTTON_H

#include <string>

#include "ui/UIElement.h"

class UIButton : public UIElement {
public:
    UIButton(const Rect &relative_rect, const std::string &text, UIManager *manager,
             UIElement *container = nullptr, const std::string &theme_block = "button");

    bool process_event(const Event &event) override;
    void draw(Surface &surface) override;

    std::string text;
    std::string theme_block;
    bool held;
};

#endif
