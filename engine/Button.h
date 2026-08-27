#ifndef ENGINE_BUTTON_H
#define ENGINE_BUTTON_H

#include <string>

#include "engine/Graphics.h"
#include "engine/Sprite.h"

/*
An enumeration class for the different states of a button click.

Author: Yash Mahmud

Attributes
----------
CLICK : int
    Represents the state when the button is clicked.
HOLD : int
    Represents the state when the button is held down.
RELEASE : int
    Represents the state when the button is released.
*/
enum class ClickState {
    CLICK = 1,
    HOLD = 2,
    RELEASE = 3
};

/*
A class used to represent a Button, which is a subclass of Sprite.

Author: Yash Mahmud
...

Attributes
----------
click_state : ClickState
    The current state of the button click.
clicked : bool
    A flag indicating whether the button is clicked or not.

Methods
-------
draw(surface):
    Draws the button on the given surface.
update():
    Updates the state of the button based on mouse events.
on_click():
    Abstract function that defines what happens when the button is clicked.
on_hold():
    Abstract function that Defines what happens when the button is held down.
on_release():
    Abstract function that Defines what happens when the button is released.
*/
class Button : public Sprite {
public:
    /*
    Author: Yash Mahmud

    Constructs all the necessary attributes for the button object.

    Parameters
    ----------
        x : int
            x-coordinate of the button
        y : int
            y-coordinate of the button
        image : str
            the filename of the button image
        scale : int, optional
            the scale of the button (default is 1)
        angle : int, optional
            the angle of the button (default is 0)
    */
    Button(double x, double y, const std::string &image, Vec2 scale = Vec2(1, 1), double angle = 0);

    /*
    Author: Yash Mahmud

    Draws the button on the given surface.

    Parameters
    ----------
        surface : Surface
            the surface to draw the button on
    */
    void draw(Surface &surface) override;

    /*
    Author: Yash Mahmud

    Updates the state of the button based on mouse events.

    Returns
    -------
        bool
            True if the button is clicked, False otherwise.
    */
    virtual bool update();

    /*
    Author: Yash Mahmud

    Defines what happens when the button is clicked.
    This method should be overridden in a subclass.
    */
    virtual void on_click() {}

    /*
    Author: Yash Mahmud

    Defines what happens when the button is held down.
    This method should be overridden in a subclass.
    */
    virtual void on_hold() {}

    /*
    Author: Yash Mahmud

    Defines what happens when the button is released.
    This method should be overridden in a subclass.
    */
    virtual void on_release() {}

    ClickState click_state;
    bool clicked;
};

#endif
