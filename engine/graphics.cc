#include "engine/graphics.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cctype>
#include <charconv>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <system_error>
#include <unordered_map>
#include <utility>

namespace engine {

void Rect::SetCenter(const Vec2& pos) {
	SetCenter(graphics::Trunc(pos.x), graphics::Trunc(pos.y));
}

Surface::Surface() : raw_(nullptr), w_(0), h_(0), owned_(false) {}

Surface::Surface(SDL_Surface* raw, int w, int h, bool owned) :
	raw_(raw), w_(w), h_(h), owned_(owned) {}

Surface::Surface(Surface&& other) noexcept :
	raw_(other.raw_), w_(other.w_), h_(other.h_), owned_(other.owned_) {
	other.raw_ = nullptr;
	other.owned_ = false;
}

Surface& Surface::operator=(Surface&& other) noexcept {
	if (this != &other) {
		if (owned_ && raw_) {
			SDL_FreeSurface(raw_);
		}
		raw_ = other.raw_;
		w_ = other.w_;
		h_ = other.h_;
		owned_ = other.owned_;
		other.raw_ = nullptr;
		other.owned_ = false;
	}
	return *this;
}

Surface::~Surface() {
	if (owned_ && raw_) {
		SDL_FreeSurface(raw_);
	}
}

Surface Surface::Create(int w, int h) {
	if (w <= 0 || h <= 0) {
		return Surface(nullptr, w < 0 ? 0 : w, h < 0 ? 0 : h, false);
	}
	SDL_Surface* raw =
		SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (!raw) {
		return Surface(nullptr, 0, 0, false);
	}
	SDL_SetSurfaceBlendMode(raw, SDL_BLENDMODE_BLEND);
	return Surface(raw, w, h, true);
}

Surface Surface::Borrow(SDL_Surface* raw) {
	if (!raw) {
		return Surface();
	}
	return Surface(raw, raw->w, raw->h, false);
}

void Surface::Blit(const Surface& src, int x, int y) {
	if (!raw_ || !src.raw_) {
		return;
	}
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = src.w_;
	dst.h = src.h_;
	SDL_UpperBlit(src.raw_, nullptr, raw_, &dst);
}

void Surface::Fill(const Color& color) {
	if (!raw_) {
		return;
	}
	SDL_FillRect(raw_, nullptr,
		SDL_MapRGBA(raw_->format, color.r, color.g, color.b, color.a));
}

void Surface::FillRgbaMult(const Color& color) {
	if (!raw_) {
		return;
	}
	SDL_LockSurface(raw_);
	Uint8* rows = static_cast<Uint8*>(raw_->pixels);
	for (int y = 0; y < h_; ++y) {
		Uint32* pixels = reinterpret_cast<Uint32*>(
			rows + static_cast<size_t>(y) * raw_->pitch);
		for (int x = 0; x < w_; ++x) {
			Uint8 dr, dg, db, da;
			SDL_GetRGBA(pixels[x], raw_->format, &dr, &dg, &db, &da);
			int nr = (dr && color.r)
				? ((static_cast<int>(dr) * color.r) + 255) >> 8
				: 0;
			int ng = (dg && color.g)
				? ((static_cast<int>(dg) * color.g) + 255) >> 8
				: 0;
			int nb = (db && color.b)
				? ((static_cast<int>(db) * color.b) + 255) >> 8
				: 0;
			int na = (da && color.a)
				? ((static_cast<int>(da) * color.a) + 255) >> 8
				: 0;
			pixels[x] = SDL_MapRGBA(raw_->format, static_cast<Uint8>(nr),
				static_cast<Uint8>(ng), static_cast<Uint8>(nb),
				static_cast<Uint8>(na));
		}
	}
	SDL_UnlockSurface(raw_);
}

namespace {

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
SDL_Texture* g_texture = nullptr;

Surface& ScreenSurface() {
	static Surface* screen = new Surface();
	return *screen;
}

std::unordered_map<std::string, Surface>& ImageCache() {
	static auto* cache = new std::unordered_map<std::string, Surface>();
	return *cache;
}

SDL_Surface* MakeSurface(int w, int h) {
	SDL_Surface* raw =
		SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (raw) {
		SDL_SetSurfaceBlendMode(raw, SDL_BLENDMODE_BLEND);
	}
	return raw;
}

Surface Rotate90(const Surface& src, int angle) {
	int numturns = (angle / 90) % 4;
	if (numturns < 0) {
		numturns = 4 + numturns;
	}

	SDL_Surface* source = src.raw();
	int dstwidth;
	int dstheight;
	if (!(numturns % 2)) {
		dstwidth = source->w;
		dstheight = source->h;
	} else {
		dstwidth = source->h;
		dstheight = source->w;
	}

	SDL_Surface* dst = MakeSurface(dstwidth, dstheight);

	SDL_LockSurface(dst);
	SDL_LockSurface(source);

	char* srcrow = static_cast<char*>(source->pixels);
	char* dstrow = static_cast<char*>(dst->pixels);
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
			srcrow +=
				((source->h - 1) * srcstepy) + ((source->w - 1) * srcstepx);
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
		char* dstpix = dstrow;
		char* srcpix = srcrow;
		for (int loopx = 0; loopx < dstwidth; ++loopx) {
			*reinterpret_cast<Uint32*>(dstpix) =
				*reinterpret_cast<Uint32*>(srcpix);
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

void Init(int width, int height, const std::string& caption) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER |
			SDL_INIT_EVENTS) != 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		std::exit(1);
	}
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	g_window = SDL_CreateWindow(caption.c_str(), SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
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

	ScreenSurface() = Surface(MakeSurface(width, height), width, height, true);
	SDL_SetSurfaceBlendMode(ScreenSurface().raw(), SDL_BLENDMODE_NONE);
}

void Quit() {
	ImageCache().clear();
	ScreenSurface() = Surface();
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

Surface& Screen() { return ScreenSurface(); }

void Update() {
	if (!ScreenSurface().raw()) {
		return;
	}
	SDL_UpdateTexture(g_texture, nullptr, ScreenSurface().raw()->pixels,
		ScreenSurface().raw()->pitch);
	SDL_RenderClear(g_renderer);
	SDL_RenderCopy(g_renderer, g_texture, nullptr, nullptr);
	SDL_RenderPresent(g_renderer);
}

int Trunc(double value) { return static_cast<int>(value); }

int Mod(int a, int b) {
	int result = a % b;
	if (result != 0 && ((result < 0) != (b < 0))) {
		result += b;
	}
	return result;
}

std::string Str(double value) {
	char buffer[64];
	std::to_chars_result result =
		std::to_chars(buffer, buffer + sizeof(buffer), value);
	std::string text(buffer, result.ptr);
	if (text.find('.') == std::string::npos &&
		text.find('e') == std::string::npos &&
		text.find("inf") == std::string::npos &&
		text.find("nan") == std::string::npos) {
		text += ".0";
	}
	return text;
}

std::string StringLower(const std::string& text) {
	std::string result;
	for (size_t i = 0; i < text.size(); ++i) {
		result += static_cast<char>(
			std::tolower(static_cast<unsigned char>(text[i])));
	}
	return result;
}

std::string StringUpper(const std::string& text) {
	std::string result;
	for (size_t i = 0; i < text.size(); ++i) {
		result += static_cast<char>(
			std::toupper(static_cast<unsigned char>(text[i])));
	}
	return result;
}

std::string ResolvePath(const std::string& path) {
	std::error_code error;
	if (std::filesystem::exists(path, error)) {
		return path;
	}

	size_t split = path.find_last_of("/\\");
	if (split == std::string::npos) {
		return path;
	}
	std::string directory = path.substr(0, split);
	std::string name = path.substr(split + 1);

	std::string lowered;
	for (size_t i = 0; i < name.size(); ++i) {
		lowered += static_cast<char>(
			std::tolower(static_cast<unsigned char>(name[i])));
	}

	std::filesystem::directory_iterator it(directory, error);
	if (error) {
		return path;
	}
	for (const std::filesystem::directory_entry& entry : it) {
		std::string candidate = entry.path().filename().string();
		std::string candidate_lowered;
		for (size_t i = 0; i < candidate.size(); ++i) {
			candidate_lowered += static_cast<char>(
				std::tolower(static_cast<unsigned char>(candidate[i])));
		}
		if (candidate_lowered == lowered) {
			return directory + "/" + candidate;
		}
	}
	return path;
}

const Surface& LoadImageFile(const std::string& path) {
	std::unordered_map<std::string, Surface>& cache = ImageCache();
	std::unordered_map<std::string, Surface>::iterator found = cache.find(path);
	if (found != cache.end()) {
		return found->second;
	}

	SDL_Surface* loaded = IMG_Load(ResolvePath(path).c_str());
	if (!loaded) {
		std::cerr << "Failed to load image " << path << ": " << IMG_GetError()
					<< std::endl;
		std::exit(1);
	}
	SDL_Surface* converted =
		SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(loaded);
	SDL_SetSurfaceBlendMode(converted, SDL_BLENDMODE_BLEND);

	Surface surface(converted, converted->w, converted->h, true);
	std::pair<std::unordered_map<std::string, Surface>::iterator, bool>
		inserted = cache.emplace(path, std::move(surface));
	return inserted.first->second;
}

Surface Scale(const Surface& src, int w, int h) {
	if (w < 0 || h < 0) {
		return Surface(nullptr, w < 0 ? 0 : w, h < 0 ? 0 : h, false);
	}
	if (w == 0 || h == 0 || src.width() == 0 || src.height() == 0) {
		return Surface(nullptr, w, h, false);
	}

	SDL_Surface* dst = MakeSurface(w, h);
	SDL_Surface* source = src.raw();

	SDL_LockSurface(dst);
	SDL_LockSurface(source);

	Uint8* srcpixels = static_cast<Uint8*>(source->pixels);
	Uint8* dstrow = static_cast<Uint8*>(dst->pixels);

	int srcpitch = source->pitch;
	int dstpitch = dst->pitch;

	int dstwidth = dst->w;
	int dstheight = dst->h;
	int srcwidth = source->w;
	int srcheight = source->h;

	long long x_step = (static_cast<long long>(srcwidth) << 16) / dstwidth;
	long long y_step = (static_cast<long long>(srcheight) << 16) / dstheight;

	long long y_pos = y_step >> 1;
	for (int looph = 0; looph < dstheight; ++looph) {
		int sy = static_cast<int>(y_pos >> 16);
		if (sy >= srcheight) {
			sy = srcheight - 1;
		}
		y_pos += y_step;

		Uint32* srcpix = reinterpret_cast<Uint32*>(
			srcpixels + static_cast<size_t>(sy) * srcpitch);
		Uint32* dstpix = reinterpret_cast<Uint32*>(dstrow);
		long long x_pos = x_step >> 1;
		for (int loopw = 0; loopw < dstwidth; ++loopw) {
			int sx = static_cast<int>(x_pos >> 16);
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

TTF_Font* LoadFont(const std::string& path, int ptsize) {
	static auto* cache = new std::map<std::pair<std::string, int>, TTF_Font*>();
	std::pair<std::string, int> key(path, ptsize);
	std::map<std::pair<std::string, int>, TTF_Font*>::iterator found =
		cache->find(key);
	if (found != cache->end()) {
		return found->second;
	}

	TTF_Font* font = TTF_OpenFont(ResolvePath(path).c_str(), ptsize);
	if (!font) {
		std::cerr << "Failed to load font " << path << ": " << TTF_GetError()
					<< std::endl;
		std::exit(1);
	}
	(*cache)[key] = font;
	return font;
}

Surface RenderText(
	TTF_Font* font, const std::string& text, const Color& color) {
	if (text.empty()) {
		return Surface(nullptr, 0, TTF_FontHeight(font), false);
	}

	SDL_Color sdl_color;
	sdl_color.r = color.r;
	sdl_color.g = color.g;
	sdl_color.b = color.b;
	sdl_color.a = color.a;

	SDL_Surface* rendered =
		TTF_RenderUTF8_Blended(font, text.c_str(), sdl_color);
	if (!rendered) {
		return Surface(nullptr, 0, TTF_FontHeight(font), false);
	}
	SDL_Surface* converted =
		SDL_ConvertSurfaceFormat(rendered, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(rendered);
	SDL_SetSurfaceBlendMode(converted, SDL_BLENDMODE_BLEND);

	return Surface(converted, converted->w, converted->h, true);
}

Surface Rotate(const Surface& src, double angle) {
	if (src.width() < 1 || src.height() < 1) {
		return Surface(nullptr, src.width(), src.height(), false);
	}

	if (!(std::fmod(static_cast<double>(static_cast<float>(angle)), 90.0))) {
		return Rotate90(src, static_cast<int>(static_cast<float>(angle)));
	}

	double radangle = angle * .01745329251994329;
	double sangle = std::sin(radangle);
	double cangle = std::cos(radangle);

	SDL_Surface* source = src.raw();

	double x = source->w;
	double y = source->h;
	double cx = cangle * x;
	double cy = cangle * y;
	double sx = sangle * x;
	double sy = sangle * y;
	int nxmax = static_cast<int>(
		std::fmax(std::fmax(std::fmax(std::fabs(cx + sy), std::fabs(cx - sy)),
						std::fabs(-cx + sy)),
			std::fabs(-cx - sy)));
	int nymax = static_cast<int>(
		std::fmax(std::fmax(std::fmax(std::fabs(sx + cy), std::fabs(sx - cy)),
						std::fabs(-sx + cy)),
			std::fabs(-sx - cy)));

	SDL_Surface* dst = MakeSurface(nxmax, nymax);
	if (!dst) {
		return Surface(nullptr, nxmax, nymax, false);
	}

	SDL_LockSurface(source);
	Uint32 bgcolor = *static_cast<Uint32*>(source->pixels);
	bgcolor &= ~source->format->Amask;

	SDL_LockSurface(dst);

	Uint8* srcpix = static_cast<Uint8*>(source->pixels);
	Uint8* dstrow = static_cast<Uint8*>(dst->pixels);
	int srcpitch = source->pitch;
	int dstpitch = dst->pitch;

	int dcy = dst->h / 2;
	int xd = ((source->w - dst->w) << 15);
	int yd = ((source->h - dst->h) << 15);

	int isin = static_cast<int>(sangle * 65536);
	int icos = static_cast<int>(cangle * 65536);

	int ax = ((dst->w) << 15) -
		static_cast<int>(cangle * ((static_cast<long long>(dst->w) - 1) << 15));
	int ay = ((dst->h) << 15) -
		static_cast<int>(sangle * ((static_cast<long long>(dst->w) - 1) << 15));

	int xmaxval = ((source->w) << 16) - 1;
	int ymaxval = ((source->h) << 16) - 1;

	for (int row = 0; row < dst->h; row++) {
		Uint32* dstpos = reinterpret_cast<Uint32*>(dstrow);
		int dx = (ax + (isin * (dcy - row))) + xd;
		int dy = (ay - (icos * (dcy - row))) + yd;
		for (int col = 0; col < dst->w; col++) {
			if (dx < 0 || dy < 0 || dx > xmaxval || dy > ymaxval) {
				*dstpos++ = bgcolor;
			} else {
				*dstpos++ = *reinterpret_cast<Uint32*>(srcpix +
					((dy >> 16) * srcpitch) +
					(static_cast<long long>(dx) >> 16 << 2));
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

}
