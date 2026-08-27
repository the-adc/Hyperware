#include "ui/Draw.h"

#include <algorithm>
#include <cmath>

namespace {

inline void blend_pixel(Uint32 *pixel, SDL_PixelFormat *format, const Color &color) {
    if (color.a == 255) {
        *pixel = SDL_MapRGBA(format, color.r, color.g, color.b, 255);
        return;
    }
    if (color.a == 0) {
        return;
    }
    Uint8 dr, dg, db, da;
    SDL_GetRGBA(*pixel, format, &dr, &dg, &db, &da);
    int alpha = color.a;
    int inverse = 255 - alpha;
    int nr = (color.r * alpha + dr * inverse) / 255;
    int ng = (color.g * alpha + dg * inverse) / 255;
    int nb = (color.b * alpha + db * inverse) / 255;
    int na = alpha + (da * inverse) / 255;
    *pixel = SDL_MapRGBA(format, (Uint8)nr, (Uint8)ng, (Uint8)nb, (Uint8)na);
}

}

namespace uidraw {

void fill_rounded_rect(Surface &dst, const Rect &area, int radius, const Color &color) {
    SDL_Surface *surface = dst.raw();
    if (!surface || area.w <= 0 || area.h <= 0) {
        return;
    }

    int limit = std::min(area.w, area.h) / 2;
    if (radius > limit) {
        radius = limit;
    }
    if (radius < 0) {
        radius = 0;
    }

    int start_x = std::max(0, area.x);
    int start_y = std::max(0, area.y);
    int end_x = std::min(surface->w, area.x + area.w);
    int end_y = std::min(surface->h, area.y + area.h);

    SDL_LockSurface(surface);
    Uint8 *rows = (Uint8 *)surface->pixels;
    double squared = (double)radius * (double)radius;

    for (int y = start_y; y < end_y; ++y) {
        Uint32 *pixels = (Uint32 *)(rows + (size_t)y * surface->pitch);
        for (int x = start_x; x < end_x; ++x) {
            if (radius > 0) {
                int cx = x;
                int cy = y;
                bool corner = false;
                double ox = 0.0;
                double oy = 0.0;
                if (cx < area.x + radius) {
                    ox = area.x + radius - 0.5;
                    corner = true;
                } else if (cx >= area.x + area.w - radius) {
                    ox = area.x + area.w - radius - 0.5;
                    corner = true;
                }
                if (cy < area.y + radius) {
                    oy = area.y + radius - 0.5;
                } else if (cy >= area.y + area.h - radius) {
                    oy = area.y + area.h - radius - 0.5;
                } else {
                    corner = false;
                }
                if (corner) {
                    double dx = cx - ox;
                    double dy = cy - oy;
                    if (dx * dx + dy * dy > squared) {
                        continue;
                    }
                }
            }
            blend_pixel(&pixels[x], surface->format, color);
        }
    }

    SDL_UnlockSurface(surface);
}

void rounded_rect_with_border(Surface &dst, const Rect &area, int radius, int border_width,
                              const Color &background, const Color &border) {
    if (border_width > 0) {
        fill_rounded_rect(dst, area, radius, border);
        Rect inner(area.x + border_width, area.y + border_width, area.w - 2 * border_width,
                   area.h - 2 * border_width);
        fill_rounded_rect(dst, inner, radius - border_width, background);
    } else {
        fill_rounded_rect(dst, area, radius, background);
    }
}

void blit_text(Surface &dst, TTF_Font *font, const std::string &text, const Color &color,
               const Rect &area, const std::string &horizontal, const std::string &vertical) {
    if (text.empty()) {
        return;
    }
    Surface rendered = graphics::render_text(font, text, color);
    if (rendered.empty()) {
        return;
    }

    int x;
    if (horizontal == "center") {
        x = area.x + (area.w - rendered.get_width()) / 2;
    } else if (horizontal == "right") {
        x = area.x + area.w - rendered.get_width();
    } else {
        x = area.x;
    }

    int y;
    if (vertical == "center") {
        y = area.y + (area.h - rendered.get_height()) / 2;
    } else if (vertical == "bottom") {
        y = area.y + area.h - rendered.get_height();
    } else {
        y = area.y;
    }

    dst.blit(rendered, x, y);
}

}
