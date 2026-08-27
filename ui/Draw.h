#ifndef UI_DRAW_H
#define UI_DRAW_H

#include <string>

#include "engine/Graphics.h"

namespace uidraw {

void fill_rounded_rect(Surface &dst, const Rect &area, int radius, const Color &color);
void rounded_rect_with_border(Surface &dst, const Rect &area, int radius, int border_width,
                              const Color &background, const Color &border);
void blit_text(Surface &dst, TTF_Font *font, const std::string &text, const Color &color,
               const Rect &area, const std::string &horizontal, const std::string &vertical);

}

#endif
