#ifndef UI_UIDROPDOWNMENU_H
#define UI_UIDROPDOWNMENU_H

#include <string>
#include <vector>

#include "engine/Events.h"
#include "ui/UIElement.h"

class UIDropDownMenu : public UIElement {
public:
    UIDropDownMenu(const std::vector<std::string> &options, const std::string &selected,
                   const Rect &relative_rect, UIManager *manager, UIElement *container = nullptr);

    bool process_event(const Event &event) override;
    bool process_overlay_event(const Event &event) override;
    void draw(Surface &surface) override;
    void draw_overlay(Surface &surface) override;

    Rect options_rect() const;

    std::vector<std::string> options;
    SelectedOption selected_option;
    bool expanded;
};

#endif
