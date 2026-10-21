#ifndef UI_UITEXTBOX_H
#define UI_UITEXTBOX_H

#include <string>

#include "ui/UIElement.h"

class UITextBox : public UIElement {
public:
    UITextBox(const std::string &html_text, const Rect &relative_rect, UIManager *manager,
              UIElement *container = nullptr);

    void draw(Surface &surface) override;

    std::string html_text;
};

#endif
