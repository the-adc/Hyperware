#ifndef ENGINE_UTILS_H_
#define ENGINE_UTILS_H_

#include <string>

namespace engine {

void MixerInit();
void MixerQuit();

// Author: Anas Tarek Qumhiyeh
//
// Returns the x-component of a vector given a distance and angle.
//
// Parameters
// ----------
//     dist : float
//         the distance of the vector
//     angle : float
//         the angle of the vector
//
// Returns
// -------
//     float
//         the x-component of the vector
double LengthDirX(double dist, double angle);

// Author: Anas Tarek Qumhiyeh
//
// Returns the y-component of a vector given a distance and angle.
//
// Parameters
// ----------
//     dist : float
//         the distance of the vector
//     angle : float
//         the angle of the vector
//
// Returns
// -------
//     float
//         the y-component of the vector
double LengthDirY(double dist, double angle);

// Author: Yash Mahmud
//
// Plays a sound effect.
//
// Parameters
// ----------
//     sound_file : str
//         the filename of the sound effect
void PlaySoundEffect(const std::string& sound_file);

// Author: Yash Mahmud
//
// Plays a music file.
//
// Parameters
// ----------
//     music_file : str
//         the filename of the music file
void PlayMusic(const std::string& music_file);

// Author: Yash Mahmud
//
// Toggles the background music.
void ToggleMusic();

}

#endif
