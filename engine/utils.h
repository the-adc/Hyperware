#ifndef HYPER_ENGINE_UTILS_H_
#define HYPER_ENGINE_UTILS_H_

#include "engine/color.h"

namespace engine {

class Util {
public:
	static constexpr double kPi = 3.14159265358979323846;

	Util() = delete;

	// Returns the x-component of a vector given a distance and angle.
	static double LengthDirX(double dist, double angle);

	// Returns the y-component of a vector given a distance and angle.
	static double LengthDirY(double dist, double angle);

	// like make_color_rgb, the numbers are cut off to whole numbers
	static Color MakeColorRgb(double r, double g, double b);

	// amount is 0 for the first colour and 1 for the second.
	static Color MergeColor(const Color& first, const Color& second,
		double amount);

	// angle in degrees from (x1, y1) to (x2, y2), between 0 and 360.
	static double PointDirection(double x1, double y1, double x2, double y2);
	static double PointDistance(double x1, double y1, double x2, double y2);

	static double Lerp(double from, double to, double amount);
	static double Clamp(double value, double low, double high);
};

}  // namespace engine

#endif  // HYPER_ENGINE_UTILS_H_
