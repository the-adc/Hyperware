#include "engine/utils.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace engine {

namespace {

std::uint8_t ToChannel(double value) {
	int channel = static_cast<int>(value);
	return static_cast<std::uint8_t>(std::clamp(channel, 0, 255));
}

}  // namespace

double Util::LengthDirX(double dist, double angle) {
	double radian_angle = angle * kPi / 180;
	return dist * std::cos(radian_angle);
}

double Util::LengthDirY(double dist, double angle) {
	double radian_angle = angle * kPi / 180;
	return dist * -std::sin(radian_angle);
}

Color Util::MakeColorRgb(double r, double g, double b) {
	return Color(ToChannel(r), ToChannel(g), ToChannel(b));
}

Color Util::MergeColor(const Color& first, const Color& second,
	double amount) {
	return Color(ToChannel(first.r + (second.r - first.r) * amount),
		ToChannel(first.g + (second.g - first.g) * amount),
		ToChannel(first.b + (second.b - first.b) * amount));
}

double Util::PointDirection(double x1, double y1, double x2, double y2) {
	double degrees = std::atan2(y1 - y2, x2 - x1) * 180.0 / kPi;
	if (degrees < 0.0) {
		degrees += 360.0;
	}
	return degrees;
}

double Util::PointDistance(double x1, double y1, double x2, double y2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	return std::sqrt(dx * dx + dy * dy);
}

double Util::Lerp(double from, double to, double amount) {
	return from + (to - from) * amount;
}

double Util::Clamp(double value, double low, double high) {
	if (value < low) {
		return low;
	}
	if (value > high) {
		return high;
	}
	return value;
}

}  // namespace engine
