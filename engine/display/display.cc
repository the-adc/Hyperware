#include "engine/display/display.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>

#include "engine/utils.h"

namespace engine {
namespace {

constexpr int kCircleSegments = 24;

sf::Vector2f Point(double x, double y) {
	return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
}

sf::Vector2u Size(int width, int height) {
	return sf::Vector2u(static_cast<unsigned int>(width),
		static_cast<unsigned int>(height));
}

sf::Color ToSf(Color color, double alpha) {
	double clamped = Util::Clamp(alpha, 0.0, 1.0);
	auto a = static_cast<std::uint8_t>(255 * clamped + 0.5);
	return sf::Color(color.r, color.g, color.b, a);
}

sf::Text MakeText(const sf::Font& font, unsigned int size,
	const std::string& value) {
	sf::String text = sf::String::fromUtf8(value.begin(), value.end());
	return sf::Text(font, text, size);
}

// left/center/right and top/middle/bottom in order of enums
const double kAlignFraction[] = {0.0, 0.5, 1.0};

}  // namespace

bool Display::Init(const std::string& title, bool start_fullscreen) {
	title_ = title;
	fullscreen_ = start_fullscreen;
	CreateWindow();
	return SetRoomSurface(view_width_, view_height_, port_width_, port_height_);
}

void Display::Shutdown() { window_.close(); }

void Display::CreateWindow() {
	if (fullscreen_) {
		window_.create(sf::VideoMode::getDesktopMode(), title_,
			sf::State::Fullscreen);
	} else {
		window_.create(sf::VideoMode(Size(port_width_, port_height_)), title_);
	}
	window_.setMouseCursorVisible(cursor_visible_);
}

bool Display::SetRoomSurface(int view_width, int view_height, int port_width,
	int port_height) {
	view_width_ = view_width;
	view_height_ = view_height;
	port_width_ = port_width;
	port_height_ = port_height;
	view_x_ = 0.0;
	view_y_ = 0.0;

	if (!surface_.resize(Size(port_width, port_height))) {
		std::cerr << "Couldn't make the application surface" << std::endl;
		return false;
	}
	UpdateSurfaceView();
	return true;
}

// the surface has the port size, the view picks the part of the room it shows
void Display::UpdateSurfaceView() {
	surface_.setView(sf::View(sf::FloatRect(Point(view_x_, view_y_),
		Point(view_width_, view_height_))));
}

void Display::SetViewPos(double x, double y) {
	view_x_ = x;
	view_y_ = y;
	UpdateSurfaceView();
}

void Display::BeginFrame() { surface_.clear(sf::Color::Black); }

void Display::EndFrame() {
	surface_.display();

	window_.clear(sf::Color::Black);
	window_.setView(LetterboxView());
	window_.draw(sf::Sprite(surface_.getTexture()));
	window_.display();
}

// shows the whole surface, with black bars if the window is a different shape
sf::View Display::LetterboxView() const {
	sf::View view(sf::FloatRect(Point(0, 0), Point(port_width_, port_height_)));

	sf::Vector2u window_size = window_.getSize();
	if (window_size.x == 0 || window_size.y == 0) {
		return view;
	}
	float window_aspect = static_cast<float>(window_size.x) / window_size.y;
	float port_aspect = static_cast<float>(port_width_) / port_height_;

	sf::FloatRect viewport({0.f, 0.f}, {1.f, 1.f});
	if (window_aspect > port_aspect) {
		viewport.size.x = port_aspect / window_aspect;
		viewport.position.x = (1.f - viewport.size.x) / 2.f;
	} else {
		viewport.size.y = window_aspect / port_aspect;
		viewport.position.y = (1.f - viewport.size.y) / 2.f;
	}
	view.setViewport(viewport);
	return view;
}

sf::RenderStates Display::States() const {
	sf::RenderStates states;
	states.blendMode =
		(blend_ == BlendMode::kAdd) ? sf::BlendAdd : sf::BlendAlpha;
	return states;
}

void Display::Sprite(assets::SpriteId id, double subimg, double x, double y) {
	SpriteExt(id, subimg, x, y, 1.0, 1.0, 0.0, C_WHITE, alpha_);
}

void Display::SpriteExt(assets::SpriteId id, double subimg, double x, double y,
	double xscale, double yscale, double rot, Color color, double alpha) {
	const assets::Sprite& data = assets::GetSprite(id);
	if (data.frames.empty()) {
		return;
	}

	// GameMaker wraps the frame number around
	int count = data.FrameCount();
	int index = static_cast<int>(std::floor(subimg)) % count;
	if (index < 0) {
		index += count;
	}

	sf::Sprite sprite(data.texture, data.frames[index].rect);
	sprite.setOrigin(Point(data.xorigin, data.yorigin));
	sprite.setPosition(Point(x, y));
	sprite.setScale(Point(xscale, yscale));
	// GameMaker rotates the other way
	sprite.setRotation(sf::degrees(static_cast<float>(-rot)));
	sprite.setColor(ToSf(color, alpha));
	surface_.draw(sprite, States());
}

void Display::SpriteTiled(assets::SpriteId id, double origin_x,
	double origin_y) {
	const assets::Sprite& data = assets::GetSprite(id);
	if (data.frames.empty() || data.width <= 0 || data.height <= 0) {
		return;
	}

	double start_x = origin_x +
		std::floor((view_x_ - origin_x) / data.width) * data.width;
	double start_y = origin_y +
		std::floor((view_y_ - origin_y) / data.height) * data.height;

	for (double y = start_y; y < view_y_ + view_height_; y += data.height) {
		for (double x = start_x; x < view_x_ + view_width_; x += data.width) {
			SpriteExt(id, 0, x, y, 1.0, 1.0, 0.0, C_WHITE, 1.0);
		}
	}
}

double Display::StringWidth(const std::string& value) const {
	if (!font_) {
		return 0.0;
	}
	return MakeText(*font_, font_size_, value).getLocalBounds().size.x;
}

double Display::StringHeight(const std::string& value) const {
	if (!font_) {
		return 0.0;
	}
	auto lines = std::count(value.begin(), value.end(), '\n') + 1;
	return lines * font_->getLineSpacing(font_size_);
}

// the lines are lined up as one block, not one by one like in GameMaker
void Display::Text(double x, double y, const std::string& value) {
	if (!font_) {
		return;
	}
	double h_fraction = kAlignFraction[static_cast<int>(halign_)];
	double v_fraction = kAlignFraction[static_cast<int>(valign_)];
	double left = x - StringWidth(value) * h_fraction;
	double top = y - StringHeight(value) * v_fraction;

	sf::Text text = MakeText(*font_, font_size_, value);
	text.setFillColor(ToSf(color_, alpha_));
	text.setPosition(Point(left, top));
	surface_.draw(text, States());
}

void Display::RectangleColor(double x1, double y1, double x2, double y2,
	Color c1, Color c2, Color c3, Color c4, bool outline) {
	// GameMaker counts the last pixel too
	double xs[] = {x1, x2 + 1, x2 + 1, x1};
	double ys[] = {y1, y1, y2 + 1, y2 + 1};
	Color colors[] = {c1, c2, c3, c4};

	sf::VertexArray shape(
		outline ? sf::PrimitiveType::LineStrip : sf::PrimitiveType::TriangleFan,
		outline ? 5 : 4);
	for (int i = 0; i < 4; ++i) {
		// an outline is all one color
		Color color = outline ? c1 : colors[i];
		shape[i] = sf::Vertex{Point(xs[i], ys[i]), ToSf(color, alpha_)};
	}
	if (outline) {
		shape[4] = shape[0];
	}
	surface_.draw(shape, States());
}

void Display::CircleColor(double x, double y, double radius, Color inner,
	Color outer, bool outline) {
	sf::VertexArray ring(sf::PrimitiveType::LineStrip, kCircleSegments + 1);
	for (int i = 0; i <= kCircleSegments; ++i) {
		double angle = 2 * Util::kPi * i / kCircleSegments;
		ring[i] = sf::Vertex{
			Point(x + std::cos(angle) * radius, y + std::sin(angle) * radius),
			ToSf(outer, alpha_)};
	}
	if (outline) {
		surface_.draw(ring, States());
		return;
	}

	sf::VertexArray fan(sf::PrimitiveType::TriangleFan, kCircleSegments + 2);
	fan[0] = sf::Vertex{Point(x, y), ToSf(inner, alpha_)};
	for (int i = 0; i <= kCircleSegments; ++i) {
		fan[i + 1] = ring[i];
	}
	surface_.draw(fan, States());
}

void Display::ToggleFullscreen() {
	fullscreen_ = !fullscreen_;
	CreateWindow();
}

void Display::SetCursorVisible(bool visible) {
	cursor_visible_ = visible;
	window_.setMouseCursorVisible(visible);
}

void Display::WindowToRoom(int window_x, int window_y, double* room_x,
	double* room_y) const {
	sf::Vector2f port =
		window_.mapPixelToCoords({window_x, window_y}, LetterboxView());
	*room_x = view_x_ + port.x * view_width_ / port_width_;
	*room_y = view_y_ + port.y * view_height_ / port_height_;
}

}  // namespace engine
