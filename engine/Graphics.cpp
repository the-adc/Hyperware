#include "engine/Graphics.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <charconv>
#include <map>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

void Rect::set_center(const Vec2 &pos) {
    set_center(graphics::trunc(pos.x), graphics::trunc(pos.y));
}

Surface::Surface() : m_raw(nullptr), m_w(0), m_h(0), m_owned(false) {}

Surface::Surface(SDL_Surface *raw, int w, int h, bool owned)
    : m_raw(raw), m_w(w), m_h(h), m_owned(owned) {}

Surface::Surface(Surface &&other) noexcept
    : m_raw(other.m_raw), m_w(other.m_w), m_h(other.m_h), m_owned(other.m_owned) {
    other.m_raw = nullptr;
    other.m_owned = false;
}

Surface &Surface::operator=(Surface &&other) noexcept {
    if (this != &other) {
        if (m_owned && m_raw) {
            SDL_FreeSurface(m_raw);
        }
        m_raw = other.m_raw;
        m_w = other.m_w;
        m_h = other.m_h;
        m_owned = other.m_owned;
        other.m_raw = nullptr;
        other.m_owned = false;
    }
    return *this;
}

Surface::~Surface() {
    if (m_owned && m_raw) {
        SDL_FreeSurface(m_raw);
    }
}

Surface Surface::create(int w, int h) {
    if (w <= 0 || h <= 0) {
        return Surface(nullptr, w < 0 ? 0 : w, h < 0 ? 0 : h, false);
    }
    SDL_Surface *raw = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
    if (!raw) {
        return Surface(nullptr, 0, 0, false);
    }
    SDL_SetSurfaceBlendMode(raw, SDL_BLENDMODE_BLEND);
    return Surface(raw, w, h, true);
}

Surface Surface::borrow(SDL_Surface *raw) {
    if (!raw) {
        return Surface();
    }
    return Surface(raw, raw->w, raw->h, false);
}

void Surface::blit(const Surface &src, int x, int y) {
    if (!m_raw || !src.m_raw) {
        return;
    }
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = src.m_w;
    dst.h = src.m_h;
    SDL_UpperBlit(src.m_raw, nullptr, m_raw, &dst);
}

void Surface::fill(const Color &color) {
    if (!m_raw) {
        return;
    }
    SDL_FillRect(m_raw, nullptr, SDL_MapRGBA(m_raw->format, color.r, color.g, color.b, color.a));
}

void Surface::fill_rgba_mult(const Color &color) {
    if (!m_raw) {
        return;
    }
    SDL_LockSurface(m_raw);
    Uint8 *rows = (Uint8 *)m_raw->pixels;
    for (int y = 0; y < m_h; ++y) {
        Uint32 *pixels = (Uint32 *)(rows + (size_t)y * m_raw->pitch);
        for (int x = 0; x < m_w; ++x) {
            Uint8 dr, dg, db, da;
            SDL_GetRGBA(pixels[x], m_raw->format, &dr, &dg, &db, &da);
            int nr = (dr && color.r) ? (((int)dr * (int)color.r) + 255) >> 8 : 0;
            int ng = (dg && color.g) ? (((int)dg * (int)color.g) + 255) >> 8 : 0;
            int nb = (db && color.b) ? (((int)db * (int)color.b) + 255) >> 8 : 0;
            int na = (da && color.a) ? (((int)da * (int)color.a) + 255) >> 8 : 0;
            pixels[x] = SDL_MapRGBA(m_raw->format, (Uint8)nr, (Uint8)ng, (Uint8)nb, (Uint8)na);
        }
    }
    SDL_UnlockSurface(m_raw);
}

namespace {

SDL_Window *g_window = nullptr;
SDL_Renderer *g_renderer = nullptr;
SDL_Texture *g_texture = nullptr;
Surface g_screen;

std::unordered_map<std::string, Surface> &image_cache() {
    static std::unordered_map<std::string, Surface> cache;
    return cache;
}

SDL_Surface *make_surface(int w, int h) {
    SDL_Surface *raw = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
    if (raw) {
        SDL_SetSurfaceBlendMode(raw, SDL_BLENDMODE_BLEND);
    }
    return raw;
}

Surface rotate90(const Surface &src, int angle) {
    int numturns = (angle / 90) % 4;
    if (numturns < 0) {
        numturns = 4 + numturns;
    }

    SDL_Surface *source = src.raw();
    int dstwidth;
    int dstheight;
    if (!(numturns % 2)) {
        dstwidth = source->w;
        dstheight = source->h;
    } else {
        dstwidth = source->h;
        dstheight = source->w;
    }

    SDL_Surface *dst = make_surface(dstwidth, dstheight);

    SDL_LockSurface(dst);
    SDL_LockSurface(source);

    char *srcrow = (char *)source->pixels;
    char *dstrow = (char *)dst->pixels;
    int srcstepx = source->format->BytesPerPixel;
    int dststepx = source->format->BytesPerPixel;
    int srcstepy = source->pitch;
    int dststepy = dst->pitch;

    switch (numturns) {
        case 1:
            srcrow += ((source->w - 1) * srcstepx);
            srcstepy = -srcstepx;
            srcstepx = source->pitch;
            break;
        case 2:
            srcrow += ((source->h - 1) * srcstepy) + ((source->w - 1) * srcstepx);
            srcstepx = -srcstepx;
            srcstepy = -srcstepy;
            break;
        case 3:
            srcrow += ((source->h - 1) * srcstepy);
            srcstepx = -srcstepy;
            srcstepy = source->format->BytesPerPixel;
            break;
        default:
            break;
    }

    for (int loopy = 0; loopy < dstheight; ++loopy) {
        char *dstpix = dstrow;
        char *srcpix = srcrow;
        for (int loopx = 0; loopx < dstwidth; ++loopx) {
            *(Uint32 *)dstpix = *(Uint32 *)srcpix;
            srcpix += srcstepx;
            dstpix += dststepx;
        }
        dstrow += dststepy;
        srcrow += srcstepy;
    }

    SDL_UnlockSurface(source);
    SDL_UnlockSurface(dst);

    return Surface(dst, dstwidth, dstheight, true);
}

}

namespace graphics {

void init(int width, int height, const std::string &caption) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        std::exit(1);
    }
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    g_window = SDL_CreateWindow(caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_SHOWN);
    if (!g_window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_renderer) {
        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE);
    }

    g_texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING, width, height);

    g_screen = Surface(make_surface(width, height), width, height, true);
    SDL_SetSurfaceBlendMode(g_screen.raw(), SDL_BLENDMODE_NONE);
}

void quit() {
    image_cache().clear();
    g_screen = Surface();
    if (g_texture) {
        SDL_DestroyTexture(g_texture);
        g_texture = nullptr;
    }
    if (g_renderer) {
        SDL_DestroyRenderer(g_renderer);
        g_renderer = nullptr;
    }
    if (g_window) {
        SDL_DestroyWindow(g_window);
        g_window = nullptr;
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

Surface &screen() {
    return g_screen;
}

void update() {
    if (!g_screen.raw()) {
        return;
    }
    SDL_UpdateTexture(g_texture, nullptr, g_screen.raw()->pixels, g_screen.raw()->pitch);
    SDL_RenderClear(g_renderer);
    SDL_RenderCopy(g_renderer, g_texture, nullptr, nullptr);
    SDL_RenderPresent(g_renderer);
}

int trunc(double value) {
    return (int)value;
}

int mod(int a, int b) {
    int result = a % b;
    if (result != 0 && ((result < 0) != (b < 0))) {
        result += b;
    }
    return result;
}

std::string str(double value) {
    char buffer[64];
    std::to_chars_result result = std::to_chars(buffer, buffer + sizeof(buffer), value);
    std::string text(buffer, result.ptr);
    if (text.find('.') == std::string::npos && text.find('e') == std::string::npos &&
        text.find("inf") == std::string::npos && text.find("nan") == std::string::npos) {
        text += ".0";
    }
    return text;
}

std::string string_lower(const std::string &text) {
    std::string result;
    for (std::size_t i = 0; i < text.size(); ++i) {
        result += (char)std::tolower((unsigned char)text[i]);
    }
    return result;
}

std::string string_upper(const std::string &text) {
    std::string result;
    for (std::size_t i = 0; i < text.size(); ++i) {
        result += (char)std::toupper((unsigned char)text[i]);
    }
    return result;
}

std::string resolve_path(const std::string &path) {
    std::error_code error;
    if (std::filesystem::exists(path, error)) {
        return path;
    }

    std::size_t split = path.find_last_of("/\\");
    if (split == std::string::npos) {
        return path;
    }
    std::string directory = path.substr(0, split);
    std::string name = path.substr(split + 1);

    std::string lowered;
    for (std::size_t i = 0; i < name.size(); ++i) {
        lowered += (char)std::tolower((unsigned char)name[i]);
    }

    std::filesystem::directory_iterator it(directory, error);
    if (error) {
        return path;
    }
    for (const std::filesystem::directory_entry &entry : it) {
        std::string candidate = entry.path().filename().string();
        std::string candidate_lowered;
        for (std::size_t i = 0; i < candidate.size(); ++i) {
            candidate_lowered += (char)std::tolower((unsigned char)candidate[i]);
        }
        if (candidate_lowered == lowered) {
            return directory + "/" + candidate;
        }
    }
    return path;
}

const Surface &load_image(const std::string &path) {
    std::unordered_map<std::string, Surface> &cache = image_cache();
    std::unordered_map<std::string, Surface>::iterator found = cache.find(path);
    if (found != cache.end()) {
        return found->second;
    }

    SDL_Surface *loaded = IMG_Load(resolve_path(path).c_str());
    if (!loaded) {
        std::cerr << "Failed to load image " << path << ": " << IMG_GetError() << std::endl;
        std::exit(1);
    }
    SDL_Surface *converted = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(loaded);
    SDL_SetSurfaceBlendMode(converted, SDL_BLENDMODE_BLEND);

    Surface surface(converted, converted->w, converted->h, true);
    std::pair<std::unordered_map<std::string, Surface>::iterator, bool> inserted =
        cache.emplace(path, std::move(surface));
    return inserted.first->second;
}

Surface scale(const Surface &src, int w, int h) {
    if (w < 0 || h < 0) {
        throw std::invalid_argument("Cannot scale to negative size");
    }
    if (w == 0 || h == 0 || src.get_width() == 0 || src.get_height() == 0) {
        return Surface(nullptr, w, h, false);
    }

    SDL_Surface *dst = make_surface(w, h);
    SDL_Surface *source = src.raw();

    SDL_LockSurface(dst);
    SDL_LockSurface(source);

    Uint8 *srcpixels = (Uint8 *)source->pixels;
    Uint8 *dstrow = (Uint8 *)dst->pixels;

    int srcpitch = source->pitch;
    int dstpitch = dst->pitch;

    int dstwidth = dst->w;
    int dstheight = dst->h;
    int srcwidth = source->w;
    int srcheight = source->h;

    long long x_step = ((long long)srcwidth << 16) / dstwidth;
    long long y_step = ((long long)srcheight << 16) / dstheight;

    long long y_pos = y_step >> 1;
    for (int looph = 0; looph < dstheight; ++looph) {
        int sy = (int)(y_pos >> 16);
        if (sy >= srcheight) {
            sy = srcheight - 1;
        }
        y_pos += y_step;

        Uint32 *srcpix = (Uint32 *)(srcpixels + (size_t)sy * srcpitch);
        Uint32 *dstpix = (Uint32 *)dstrow;
        long long x_pos = x_step >> 1;
        for (int loopw = 0; loopw < dstwidth; ++loopw) {
            int sx = (int)(x_pos >> 16);
            if (sx >= srcwidth) {
                sx = srcwidth - 1;
            }
            x_pos += x_step;
            *dstpix++ = srcpix[sx];
        }
        dstrow += dstpitch;
    }

    SDL_UnlockSurface(source);
    SDL_UnlockSurface(dst);

    return Surface(dst, w, h, true);
}

TTF_Font *load_font(const std::string &path, int ptsize) {
    static std::map<std::pair<std::string, int>, TTF_Font *> cache;
    std::pair<std::string, int> key(path, ptsize);
    std::map<std::pair<std::string, int>, TTF_Font *>::iterator found = cache.find(key);
    if (found != cache.end()) {
        return found->second;
    }

    TTF_Font *font = TTF_OpenFont(resolve_path(path).c_str(), ptsize);
    if (!font) {
        std::cerr << "Failed to load font " << path << ": " << TTF_GetError() << std::endl;
        std::exit(1);
    }
    cache[key] = font;
    return font;
}

Surface render_text(TTF_Font *font, const std::string &text, const Color &color) {
    if (text.empty()) {
        return Surface(nullptr, 0, TTF_FontHeight(font), false);
    }

    SDL_Color sdl_color;
    sdl_color.r = color.r;
    sdl_color.g = color.g;
    sdl_color.b = color.b;
    sdl_color.a = color.a;

    SDL_Surface *rendered = TTF_RenderUTF8_Blended(font, text.c_str(), sdl_color);
    if (!rendered) {
        return Surface(nullptr, 0, TTF_FontHeight(font), false);
    }
    SDL_Surface *converted = SDL_ConvertSurfaceFormat(rendered, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(rendered);
    SDL_SetSurfaceBlendMode(converted, SDL_BLENDMODE_BLEND);

    return Surface(converted, converted->w, converted->h, true);
}

Surface rotate(const Surface &src, double angle) {
    if (src.get_width() < 1 || src.get_height() < 1) {
        return Surface(nullptr, src.get_width(), src.get_height(), false);
    }

    if (!(std::fmod((double)(float)angle, 90.0))) {
        return rotate90(src, (int)(float)angle);
    }

    double radangle = angle * .01745329251994329;
    double sangle = std::sin(radangle);
    double cangle = std::cos(radangle);

    SDL_Surface *source = src.raw();

    double x = source->w;
    double y = source->h;
    double cx = cangle * x;
    double cy = cangle * y;
    double sx = sangle * x;
    double sy = sangle * y;
    int nxmax = (int)(std::fmax(std::fmax(std::fmax(std::fabs(cx + sy), std::fabs(cx - sy)),
                                          std::fabs(-cx + sy)),
                                std::fabs(-cx - sy)));
    int nymax = (int)(std::fmax(std::fmax(std::fmax(std::fabs(sx + cy), std::fabs(sx - cy)),
                                          std::fabs(-sx + cy)),
                                std::fabs(-sx - cy)));

    SDL_Surface *dst = make_surface(nxmax, nymax);
    if (!dst) {
        return Surface(nullptr, nxmax, nymax, false);
    }

    SDL_LockSurface(source);
    Uint32 bgcolor = *(Uint32 *)source->pixels;
    bgcolor &= ~source->format->Amask;

    SDL_LockSurface(dst);

    Uint8 *srcpix = (Uint8 *)source->pixels;
    Uint8 *dstrow = (Uint8 *)dst->pixels;
    int srcpitch = source->pitch;
    int dstpitch = dst->pitch;

    int dcy = dst->h / 2;
    int xd = ((source->w - dst->w) << 15);
    int yd = ((source->h - dst->h) << 15);

    int isin = (int)(sangle * 65536);
    int icos = (int)(cangle * 65536);

    int ax = ((dst->w) << 15) - (int)(cangle * (((long long)dst->w - 1) << 15));
    int ay = ((dst->h) << 15) - (int)(sangle * (((long long)dst->w - 1) << 15));

    int xmaxval = ((source->w) << 16) - 1;
    int ymaxval = ((source->h) << 16) - 1;

    for (int row = 0; row < dst->h; row++) {
        Uint32 *dstpos = (Uint32 *)dstrow;
        int dx = (ax + (isin * (dcy - row))) + xd;
        int dy = (ay - (icos * (dcy - row))) + yd;
        for (int col = 0; col < dst->w; col++) {
            if (dx < 0 || dy < 0 || dx > xmaxval || dy > ymaxval) {
                *dstpos++ = bgcolor;
            } else {
                *dstpos++ = *(Uint32 *)(srcpix + ((dy >> 16) * srcpitch) +
                                        ((long long)dx >> 16 << 2));
            }
            dx += icos;
            dy += isin;
        }
        dstrow += dstpitch;
    }

    SDL_UnlockSurface(dst);
    SDL_UnlockSurface(source);

    return Surface(dst, nxmax, nymax, true);
}

}
