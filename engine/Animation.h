#ifndef ENGINE_ANIMATION_H
#define ENGINE_ANIMATION_H

#include <functional>
#include <vector>

/*
A class to represent an animation.

Author: Anas Tarek Qumhiyeh

Attributes
----------
frames : list
    A list of frames to animate.
delay : int
    The delay between each frame.
loop : bool
    A flag indicating whether the animation should loop.
play : bool
    A flag indicating whether the animation is playing.
update_func : function
    The function to call when updating the animation.
current_frame : int
    The current frame of the animation.
current_delay : int
    The current delay of the animation.

Methods
-------
update():
    Updates the animation.
on_update():
    A function to call when updating the animation.
play_animation():
    Plays the animation.
play_animation_at(frame: int):
    Plays the animation at a specific frame.
get_current_frame():
    Returns the current frame of the animation.
is_playing() -> bool:
    Checks if the animation is playing.
ended() -> bool:
    Checks if the animation has ended.
set_on_update(func):
    Sets the function to call when updating the animation.
*/
class Animation {
public:
    /*
    Author: Anas Tarek Qumhiyeh

    Constructs all the necessary attributes for the Animation object.

    Parameters
    ----------
        frames : list
            A list of frames to animate.
        delay : int
            The delay between each frame.
        loop : bool, optional
            A flag indicating whether the animation should loop (default is False).
    */
    Animation(const std::vector<double> &frames, int delay, bool loop = false);

    /*
    Author: Anas Tarek Qumhiyeh

    Updates the animation.
    */
    void update();

    /*
    Author: Anas Tarek Qumhiyeh

    A function to call when updating the animation.
    */
    void on_update();

    /*
    Author: Anas Tarek Qumhiyeh

    Plays the animation.
    */
    void play_animation();

    /*
    Author: Anas Tarek Qumhiyeh

    Plays the animation at a specific frame.

    Parameters
    ----------
        frame : int
            The frame to play the animation at.
    */
    void play_animation_at(int frame);

    /*
    Author: Anas Tarek Qumhiyeh

    Returns the current frame of the animation.

    Returns
    -------
        int
            The current frame of the animation.
    */
    double get_current_frame();

    /*
    Author: Anas Tarek Qumhiyeh

    Checks if the animation is playing.

    Returns
    -------
        bool
            True if the animation is playing, False otherwise.
    */
    bool is_playing();

    /*
    Author: Anas Tarek Qumhiyeh

    Checks if the animation has ended.

    Returns
    -------
        bool
            True if the animation has ended, False otherwise.
    */
    bool ended();

    /*
    Author: Anas Tarek Qumhiyeh

    Sets the function to call when updating the animation.

    Parameters
    ----------
        func : function
            The function to call when updating the animation.
    */
    void set_on_update(const std::function<void()> &func);

    std::vector<double> frames;
    int delay;
    bool loop;
    bool play;
    std::function<void()> update_func;
    int current_frame;
    int current_delay;
};

#endif
