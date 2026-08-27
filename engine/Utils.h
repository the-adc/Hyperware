#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <string>

void mixer_init();
void mixer_quit();

/*
Author: Anas Tarek Qumhiyeh

Returns the x-component of a vector given a distance and angle.

Parameters
----------
    dist : float
        the distance of the vector
    angle : float
        the angle of the vector

Returns
-------
    float
        the x-component of the vector
*/
double lengthdir_x(double dist, double angle);

/*
Author: Anas Tarek Qumhiyeh

Returns the y-component of a vector given a distance and angle.

Parameters
----------
    dist : float
        the distance of the vector
    angle : float
        the angle of the vector

Returns
-------
    float
        the y-component of the vector
*/
double lengthdir_y(double dist, double angle);

/*
Author: Yash Mahmud

Plays a sound effect.

Parameters
----------
    sound_file : str
        the filename of the sound effect
*/
void play_sound(const std::string &sound_file);

/*
Author: Yash Mahmud

Plays a music file.

Parameters
----------
    music_file : str
        the filename of the music file
*/
void play_music(const std::string &music_file);

/*
Author: Yash Mahmud

Toggles the background music.
*/
void toggle_music();

#endif
