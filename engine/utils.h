#ifndef HYPER_ENGINE_UTILS_H_
#define HYPER_ENGINE_UTILS_H_

#include <string>

#include "engine/color.h"

namespace engine {

class Util {
public:
	static constexpr double kPi = 3.14159265358979323846;

	Util() = delete;

	static void MixerInit();
	static void MixerQuit();

	// Returns the x-component of a vector given a distance and angle.
	static double LengthDirX(double dist, double angle);

	// Returns the y-component of a vector given a distance and angle.
	static double LengthDirY(double dist, double angle);

	// Cuts the components off to whole numbers, like GameMaker's make_color_rgb.
	static Color MakeColorRgb(double r, double g, double b);

	// amount is 0 for the first colour and 1 for the second.
	static Color MergeColor(const Color& first, const Color& second,
		double amount);

	// angle in degrees from (x1, y1) to (x2, y2), between 0 and 360.
	static double PointDirection(double x1, double y1, double x2, double y2);
	static double PointDistance(double x1, double y1, double x2, double y2);

	static double Lerp(double from, double to, double amount);
	static double Clamp(double value, double low, double high);

	// Plays a sound effect.
	static void PlaySFX(const std::string& sound_file);

	// Plays a music file.
	static void PlayMusic(const std::string& music_file);

	// Toggles the background music.
	static void ToggleMusic();
};

}  // namespace engine

#endif  // HYPER_ENGINE_UTILS_H_
