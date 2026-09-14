#ifndef HYPER_ENGINE_DISPLAY_DISPLAY_H_
#define HYPER_ENGINE_DISPLAY_DISPLAY_H_

#include <SFML/Graphics.hpp>

#include <string>

#include "engine/assets.h"
#include "engine/color.h"

namespace engine {

// Everything the game draws goes through here, kinda like GameMaker's application_surface.
class Display {
public:
	enum class HAlign { kLeft, kCenter, kRight };
	enum class VAlign { kTop, kMiddle, kBottom };
	enum class BlendMode { kNormal, kAdd };

	bool Init(const std::string& title, bool start_fullscreen, bool hidden);
	void Shutdown();

	sf::RenderWindow& window() { return window_; }

	// camera (view) and the size it is scaled to (port), call on every map start
	bool SetRoomSurface(int view_width, int view_height, int port_width,
		int port_height);

	void BeginFrame();
	void EndFrame();

	void SetViewPos(double x, double y);
	double ViewX() const { return view_x_; }
	double ViewY() const { return view_y_; }
	int ViewWidth() const { return view_width_; }
	int ViewHeight() const { return view_height_; }

	void SetColor(Color color) { color_ = color; }
	void SetAlpha(double alpha) { alpha_ = alpha; }
	void SetHalign(HAlign align) { halign_ = align; }
	void SetValign(VAlign align) { valign_ = align; }
	void SetFont(const sf::Font* font) { font_ = font; }
	void SetFontSize(unsigned int size) { font_size_ = size; }
	void SetBlendmode(BlendMode mode) { blend_ = mode; }

	void Sprite(assets::SpriteId id, double subimg, double x, double y);
	void SpriteExt(assets::SpriteId id, double subimg, double x, double y,
		double xscale, double yscale, double rot, Color color, double alpha);
	void SpriteTiled(assets::SpriteId id, double origin_x, double origin_y);

	void Text(double x, double y, const std::string& value);
	double StringWidth(const std::string& value) const;
	double StringHeight(const std::string& value) const;

	void RectangleColor(double x1, double y1, double x2, double y2, Color c1,
		Color c2, Color c3, Color c4, bool outline);
	void CircleColor(double x, double y, double radius, Color inner, Color outer,
		bool outline);

	void ToggleFullscreen();

	// for the mouse
	void WindowToRoom(int window_x, int window_y, double* room_x,
		double* room_y) const;

private:
	void CreateWindow();
	void UpdateSurfaceView();
	sf::View LetterboxView() const;
	sf::RenderStates States() const;

	sf::RenderWindow window_;
	sf::RenderTexture surface_;
	std::string title_;
	bool fullscreen_ = false;

	int view_width_ = 640;
	int view_height_ = 480;
	int port_width_ = 640;
	int port_height_ = 480;
	double view_x_ = 0.0;
	double view_y_ = 0.0;

	Color color_ = C_WHITE;
	double alpha_ = 1.0;
	HAlign halign_ = HAlign::kLeft;
	VAlign valign_ = VAlign::kTop;
	BlendMode blend_ = BlendMode::kNormal;
	const sf::Font* font_ = nullptr;
	unsigned int font_size_ = 16;
};

}  // namespace engine

#endif  // HYPER_ENGINE_DISPLAY_DISPLAY_H_
