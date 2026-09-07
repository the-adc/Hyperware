#ifndef HYPER_ENGINE_BUTTON_H_
#define HYPER_ENGINE_BUTTON_H_

#include <string>

#include "engine/graphics.h"
#include "engine/sprite.h"

namespace engine {

// An enumeration class for the different states of a button click.
//
// Author: Yash Mahmud
//
// kClick:
//     Represents the state when the button is clicked.
// kHold:
//     Represents the state when the button is held down.
// kRelease:
//     Represents the state when the button is released.
enum class ClickState { kClick = 1, kHold = 2, kRelease = 3 };

// A class used to represent a Button, which is a subclass of Sprite.
//
// Author: Yash Mahmud
class Button : public Sprite {
public:
	// Author: Yash Mahmud
	//
	// Constructs all the necessary attributes for the button object.
	//
	// Parameters
	// ----------
	//     x : int
	//         x-coordinate of the button
	//     y : int
	//         y-coordinate of the button
	//     image : str
	//         the filename of the button image
	//     scale : int, optional
	//         the scale of the button (default is 1)
	//     angle : int, optional
	//         the angle of the button (default is 0)
	Button(double x, double y, const std::string& image,
		const Vec2& scale = Vec2(1, 1), double angle = 0);

	// Author: Yash Mahmud
	//
	// Draws the button on the given surface.
	//
	// Parameters
	// ----------
	//     surface : Surface
	//         the surface to draw the button on
	void Draw(Surface& surface) override;

	// Author: Yash Mahmud
	//
	// Updates the state of the button based on mouse events.
	//
	// Returns
	// -------
	//     bool
	//         True if the button is clicked, False otherwise.
	virtual bool Update();

	// Author: Yash Mahmud
	//
	// Defines what happens when the button is clicked.
	// This method should be overridden in a subclass.
	virtual void OnClick() {}

	// Author: Yash Mahmud
	//
	// Defines what happens when the button is held down.
	// This method should be overridden in a subclass.
	virtual void OnHold() {}

	// Author: Yash Mahmud
	//
	// Defines what happens when the button is released.
	// This method should be overridden in a subclass.
	virtual void OnRelease() {}

	ClickState click_state() const { return click_state_; }
	bool clicked() const { return clicked_; }

private:
	ClickState click_state_;
	bool clicked_;
};

}  // namespace engine

#endif  // HYPER_ENGINE_BUTTON_H_
