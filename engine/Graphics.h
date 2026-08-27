#ifndef ENGINE_GRAPHICS_H
#define ENGINE_GRAPHICS_H

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

#include "engine/Vec2.h"

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    Color() : r(0), g(0), b(0), a(255) {}
    Color(int r, int g, int b) : r((Uint8)r), g((Uint8)g), b((Uint8)b), a(255) {}
    Color(int r, int g, int b, int a) : r((Uint8)r), g((Uint8)g), b((Uint8)b), a((Uint8)a) {}
};

struct Rect {
    int x;
    int y;
    int w;
    int h;

    Rect() : x(0), y(0), w(0), h(0) {}
    Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

    bool collidepoint(int px, int py) const {
        return px >= x && px < x + w && py >= y && py < y + h;
    }

    void set_center(int cx, int cy) {
        x = cx - (w / 2);
        y = cy - (h / 2);
    }

    void set_center(const Vec2 &pos);

    int centerx() const { return x + (w / 2); }
    int centery() const { return y + (h / 2); }
};

class Surface {
public:
    Surface();
    Surface(SDL_Surface *raw, int w, int h, bool owned);
    Surface(Surface &&other) noexcept;
    Surface &operator=(Surface &&other) noexcept;
    ~Surface();

    Surface(const Surface &) = delete;
    Surface &operator=(const Surface &) = delete;

    static Surface create(int w, int h);
    static Surface borrow(SDL_Surface *raw);

    int get_width() const { return m_w; }
    int get_height() const { return m_h; }
    Rect get_rect() const { return Rect(0, 0, m_w, m_h); }
    SDL_Surface *raw() const { return m_raw; }
    bool empty() const { return m_raw == nullptr; }

    void blit(const Surface &src, int x, int y);
    void fill(const Color &color);
    void fill_rgba_mult(const Color &color);

private:
    SDL_Surface *m_raw;
    int m_w;
    int m_h;
    bool m_owned;
};

namespace graphics {

void init(int width, int height, const std::string &caption);
void quit();

Surface &screen();
void update();

int trunc(double value);
int mod(int a, int b);
std::string str(double value);
std::string string_lower(const std::string &text);
std::string string_upper(const std::string &text);
std::string resolve_path(const std::string &path);

const Surface &load_image(const std::string &path);
Surface scale(const Surface &src, int w, int h);
Surface rotate(const Surface &src, double angle);

TTF_Font *load_font(const std::string &path, int ptsize);
Surface render_text(TTF_Font *font, const std::string &text, const Color &color);

}

#endif
