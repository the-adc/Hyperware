#ifndef ENGINE_TEXT_H
#define ENGINE_TEXT_H

#include <string>

#include "engine/Graphics.h"
#include "engine/Vec2.h"

/*
A class used to represent a Text.

Author: Yash Mahmud

...

Attributes
----------
font : Font
    the font of the text
pos : tuple
    a tuple representing the position of the text (x, y)
color : Color
    the color of the text
text : Surface
    the rendered text

Methods
-------
draw(surface):
    Draws the text on the given surface.
set_text(text):
    Sets the text to be rendered.
set_alignment(alignment):
    Sets the alignment of the text.
*/
class Text {
public:
    /*
    Author: Yash Mahmud

    Constructs all the necessary attributes for the text object.

    Parameters
    ----------
        text : str
            the text to be rendered
        pos : tuple
            a tuple representing the position of the text (x, y)
        color : Color
            the color of the text
        font : str
            the filename of the font
        alignment : str, optional
            the alignment of the text (default is 'topleft')
    */
    Text(const std::string &text, const Vec2 &pos, const Color &color, const std::string &font,
         const std::string &alignment = "topleft");

    /*
    Author: Yash Mahmud

    Draws the text on the given surface.

    Parameters
    ----------
        surface : Surface
            the surface to draw the text on
    */
    void draw(Surface &surface);

    /*
    Author: Yash Mahmud

    Sets the text to be rendered.

    Parameters
    ----------
        text : str
            the new text to be rendered
    */
    void set_text(const std::string &text);

    /*
    Author: Yash Mahmud

    Sets the alignment of the text.

    Parameters
    ----------
        alignment : str
            the alignment of the text
    */
    void set_alignment(const std::string &alignment);

    TTF_Font *font;
    Vec2 pos;
    Color color;
    Surface text;
    std::string alignment;
    Rect text_rect;
};

#endif
