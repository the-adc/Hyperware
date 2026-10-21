#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

#include <array>
#include <string>

#include "engine/Graphics.h"
#include "engine/Vec2.h"

/*
A class used to represent a Sprite.

Author: Yash Mahmud

...

Attributes
----------
pos : tuple
    a tuple representing the position of the sprite (x, y)
scale : int
    the scale of the sprite
angle : int
    the angle of the sprite
image : Surface
    the image of the sprite
rect : Rect
    the rectangle area of the sprite

Methods
-------
draw(surface):
    Draws the sprite on the given surface.
set_sprite(image):
    Sets the sprite image and its rectangle area.
set_scale(xscale, yscale):
    Sets the scale of the sprite.
set_position(pos):
    Sets the position of the sprite.
set_position_smooth(pos, amount):
    Spherically interpolates the position of the sprite.
*/
class Sprite {
public:
    /*
    Author: Yash Mahmud

    Constructs all the necessary attributes for the sprite object.

    Parameters
    ----------
        x : int
            x-coordinate of the sprite
        y : int
            y-coordinate of the sprite
        image : str
            the filename of the sprite image
        scale : tuple, optional
            the scale of the sprite (default is (1, 1))
        angle : int, optional
            the angle of the sprite (default is 0)
    */
    Sprite(double x, double y, const std::string &image, Vec2 scale = Vec2(1, 1), double angle = 0);

    virtual ~Sprite() {}

    /*
    Author: Yash Mahmud

    Draws the sprite on the given surface.

    Parameters
    ----------
        surface : Surface
            the surface to draw the sprite on
    */
    virtual void draw(Surface &surface);

    /*
    Author: Yash Mahmud

    Sets the sprite image and its rectangle area.

    Parameters
    ----------
        image : str
            the filename of the sprite image

    Returns
    -------
        tuple
            a tuple containing the sprite image and its rect
    */
    void set_sprite(const std::string &image);

    /*
    Author: Yash Mahmud

    Sets the scale of the sprite.

    Parameters
    ----------
        xscale : int
            the x-scale of the sprite
        yscale : int
            the y-scale of the sprite
    */
    void set_scale(double xscale, double yscale);

    /*
    Author: Yash Mahmud

    Sets the position of the sprite.

    Parameters
    ----------
        pos : tuple
            a tuple representing the new position of the sprite (x, y)
    */
    void set_position(const Vec2 &pos);

    /*
    Author: Yash Mahmud

    Spherically interpolates the position of the sprite.

    Parameters
    ----------
        pos : tuple
            a tuple representing the new position of the sprite (x, y)
        amount : float
            a float representing the amount of interpolation (the more the value,
            the faster the sprite will move.)
    */
    void set_position_smooth(const Vec2 &pos, double amount);

    std::array<int, 2> size;
    Vec2 pos;
    Vec2 scale;
    double angle;
    Surface image;
    Rect rect;
    std::string filename;
};

#endif
