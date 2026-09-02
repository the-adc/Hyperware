#ifndef ENGINE_VEC2_H_
#define ENGINE_VEC2_H_

#include <cmath>
#include <optional>

namespace engine {

inline constexpr double kPi = 3.14159265358979323846;

struct Vec2 {
	double x;
	double y;

	Vec2() : x(0.0), y(0.0) {}
	Vec2(double x, double y) : x(x), y(y) {}

	static constexpr double kEpsilon = 1e-6;

	double& operator[](int i) { return i == 0 ? x : y; }
	const double& operator[](int i) const { return i == 0 ? x : y; }

	Vec2 operator+(const Vec2& other) const {
		return Vec2(x + other.x, y + other.y);
	}
	Vec2 operator-(const Vec2& other) const {
		return Vec2(x - other.x, y - other.y);
	}
	Vec2 operator*(double s) const { return Vec2(x * s, y * s); }
	Vec2 operator/(double s) const { return Vec2(x / s, y / s); }
	Vec2 operator-() const { return Vec2(-x, -y); }

	Vec2& operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2& operator-=(const Vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	bool operator==(const Vec2& other) const {
		return std::fabs(x - other.x) < kEpsilon &&
			std::fabs(y - other.y) < kEpsilon;
	}
	bool operator!=(const Vec2& other) const { return !(*this == other); }

	Vec2 FloorDiv(double s) const {
		return Vec2(std::floor(x / s), std::floor(y / s));
	}

	double Length() const { return std::sqrt(x * x + y * y); }

	static std::optional<Vec2> Slerp(
		const Vec2& from, const Vec2& to, double t);
};

inline std::optional<Vec2> Vec2::Slerp(
	const Vec2& from, const Vec2& to, double t) {
	double length1 = from.Length();
	double length2 = to.Length();
	if (length1 < kEpsilon || length2 < kEpsilon) {
		return std::nullopt;
	}
	if (t < -1.0 || t > 1.0) {
		return std::nullopt;
	}

	double tmp = (from.x * to.x + from.y * to.y) / (length1 * length2);
	tmp = (tmp < -1.0 ? -1.0 : (tmp > 1.0 ? 1.0 : tmp));

	double angle;
	if (t < 0.0) {
		angle = std::acos(tmp) - 2.0 * kPi;
		t = -t;
	} else {
		angle = std::acos(tmp);
	}

	if (angle < kEpsilon || std::fabs(angle - 2.0 * kPi) < kEpsilon) {
		return Vec2(
			from.x * (1.0 - t) + to.x * t, from.y * (1.0 - t) + to.y * t);
	}
	if (std::fabs(angle - kPi) < kEpsilon) {
		return std::nullopt;
	}

	double f0 = ((length2 - length1) * t + length1) / std::sin(angle);
	double f1 = std::sin(angle * (1.0 - t)) / length1;
	double f2 = std::sin(angle * t) / length2;

	return Vec2((from.x * f1 + to.x * f2) * f0, (from.y * f1 + to.y * f2) * f0);
}

}

#endif
