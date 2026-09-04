#ifndef HYPER_ENGINE_GRAPHICS_H_
#define HYPER_ENGINE_GRAPHICS_H_

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

#include "engine/color.h"
#include "engine/vec2.h"

namespace engine {

struct Rect {
	int x;
	int y;
	int w;
	int h;

	Rect() : x(0), y(0), w(0), h(0) {}
	Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

	bool CollidePoint(int px, int py) const {
		return px >= x && px < x + w && py >= y && py < y + h;
	}

	void SetCenter(int cx, int cy) {
		x = cx - (w / 2);
		y = cy - (h / 2);
	}

	void SetCenter(const Vec2& pos);

	int center_x() const { return x + (w / 2); }
	int center_y() const { return y + (h / 2); }
};

class Surface {
public:
	Surface();
	Surface(SDL_Surface* raw, int w, int h, bool owned);
	Surface(Surface&& other) noexcept;
	Surface& operator=(Surface&& other) noexcept;
	~Surface();

	Surface(const Surface&) = delete;
	Surface& operator=(const Surface&) = delete;

	static Surface Create(int w, int h);
	static Surface Borrow(SDL_Surface* raw);

	int width() const { return w_; }
	int height() const { return h_; }
	Rect rect() const { return Rect(0, 0, w_, h_); }
	SDL_Surface* raw() const { return raw_; }
	bool empty() const { return raw_ == nullptr; }

	void Blit(const Surface& src, int x, int y);
	void Fill(const Color& color);
	void FillRgbaMult(const Color& color);

private:
	SDL_Surface* raw_;
	int w_;
	int h_;
	bool owned_;
};

namespace graphics {

void Init(int width, int height, const std::string& caption);
void Quit();

Surface& Screen();
void Update();

int Trunc(double value);
int Mod(int a, int b);
std::string Str(double value);
std::string StringLower(const std::string& text);
std::string StringUpper(const std::string& text);
std::string ResolvePath(const std::string& path);

const Surface& LoadImageFile(const std::string& path);
Surface Scale(const Surface& src, int w, int h);
Surface Rotate(const Surface& src, double angle);

TTF_Font* LoadFont(const std::string& path, int ptsize);
Surface RenderText(TTF_Font* font, const std::string& text, const Color& color);

}  // namespace graphics

}  // namespace engine

#endif  // HYPER_ENGINE_GRAPHICS_H_
