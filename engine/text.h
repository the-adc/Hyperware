#ifndef ENGINE_TEXT_H_
#define ENGINE_TEXT_H_

#include <string>

#include "engine/graphics.h"
#include "engine/vec2.h"

namespace engine {

enum class Alignment {
	kCenter,
	kTopLeft,
	kTopRight,
	kBottomLeft,
	kBottomRight
};

// A class used to represent a Text.
//
// Author: Yash Mahmud
class Text {
public:
	// Author: Yash Mahmud
	//
	// Constructs all the necessary attributes for the text object.
	//
	// Parameters
	// ----------
	//     text : str
	//         the text to be rendered
	//     pos : tuple
	//         a tuple representing the position of the text (x, y)
	//     color : Color
	//         the color of the text
	//     font : str
	//         the filename of the font
	//     alignment : Alignment, optional
	//         the alignment of the text (default is kTopLeft)
	Text(const std::string& text, const Vec2& pos, const Color& color,
		const std::string& font, Alignment alignment = Alignment::kTopLeft);

	Text(const Text&) = delete;
	Text& operator=(const Text&) = delete;
	Text(Text&&) = default;
	Text& operator=(Text&&) = default;

	// Author: Yash Mahmud
	//
	// Draws the text on the given surface.
	//
	// Parameters
	// ----------
	//     surface : Surface
	//         the surface to draw the text on
	void Draw(Surface& surface);

	// Author: Yash Mahmud
	//
	// Sets the text to be rendered.
	//
	// Parameters
	// ----------
	//     text : str
	//         the new text to be rendered
	void SetText(const std::string& text);

	// Author: Yash Mahmud
	//
	// Sets the alignment of the text.
	//
	// Parameters
	// ----------
	//     alignment : Alignment
	//         the alignment of the text
	void SetAlignment(Alignment alignment);

	void SetPosition(const Vec2& pos);

	TTF_Font* font() const { return font_; }
	const Vec2& pos() const { return pos_; }
	const Color& color() const { return color_; }
	const Surface& text() const { return text_; }
	Alignment alignment() const { return alignment_; }
	const Rect& text_rect() const { return text_rect_; }

private:
	TTF_Font* font_;
	Vec2 pos_;
	Color color_;
	Surface text_;
	Alignment alignment_;
	Rect text_rect_;
};

}

#endif
