#ifndef ENGINE_SPRITE_H_
#define ENGINE_SPRITE_H_

#include <array>
#include <string>

#include "engine/graphics.h"
#include "engine/vec2.h"

namespace engine {

// A class used to represent a Sprite.
//
// Author: Yash Mahmud
class Sprite {
public:
	// Author: Yash Mahmud
	//
	// Constructs all the necessary attributes for the sprite object.
	//
	// Parameters
	// ----------
	//     x : int
	//         x-coordinate of the sprite
	//     y : int
	//         y-coordinate of the sprite
	//     image : str
	//         the filename of the sprite image
	//     scale : tuple, optional
	//         the scale of the sprite (default is (1, 1))
	//     angle : int, optional
	//         the angle of the sprite (default is 0)
	Sprite(double x, double y, const std::string& image,
		const Vec2& scale = Vec2(1, 1), double angle = 0);

	Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;
	Sprite(Sprite&&) = default;
	Sprite& operator=(Sprite&&) = default;

	virtual ~Sprite() = default;

	// Author: Yash Mahmud
	//
	// Draws the sprite on the given surface.
	//
	// Parameters
	// ----------
	//     surface : Surface
	//         the surface to draw the sprite on
	virtual void Draw(Surface& surface);

	// Author: Yash Mahmud
	//
	// Sets the sprite image and its rectangle area.
	//
	// Parameters
	// ----------
	//     image : str
	//         the filename of the sprite image
	//
	// Returns
	// -------
	//     tuple
	//         a tuple containing the sprite image and its rect
	void SetSprite(const std::string& image);

	// Author: Yash Mahmud
	//
	// Sets the scale of the sprite.
	//
	// Parameters
	// ----------
	//     xscale : int
	//         the x-scale of the sprite
	//     yscale : int
	//         the y-scale of the sprite
	void SetScale(double xscale, double yscale);

	// Author: Yash Mahmud
	//
	// Sets the position of the sprite.
	//
	// Parameters
	// ----------
	//     pos : tuple
	//         a tuple representing the new position of the sprite (x, y)
	void SetPosition(const Vec2& pos);

	// Author: Yash Mahmud
	//
	// Spherically interpolates the position of the sprite.
	//
	// Parameters
	// ----------
	//     pos : tuple
	//         a tuple representing the new position of the sprite (x, y)
	//     amount : float
	//         a float representing the amount of interpolation (the more the
	//         value, the faster the sprite will move.)
	void SetPositionSmooth(const Vec2& pos, double amount);

	const std::array<int, 2>& size() const { return size_; }
	const Vec2& pos() const { return pos_; }
	const Vec2& scale() const { return scale_; }
	double angle() const { return angle_; }
	const Surface& image() const { return image_; }
	const Rect& rect() const { return rect_; }
	const std::string& filename() const { return filename_; }

private:
	std::array<int, 2> size_;
	Vec2 pos_;
	Vec2 scale_;
	double angle_;
	Surface image_;
	Rect rect_;
	std::string filename_;
};

}

#endif
