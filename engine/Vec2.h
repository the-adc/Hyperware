#ifndef ENGINE_VEC2_H
#define ENGINE_VEC2_H

#include <cmath>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Vec2 {
public:
    double x;
    double y;

    Vec2() : x(0.0), y(0.0) {}
    Vec2(double x, double y) : x(x), y(y) {}

    static constexpr double EPSILON = 1e-6;

    double &operator[](int i) { return i == 0 ? x : y; }
    const double &operator[](int i) const { return i == 0 ? x : y; }

    Vec2 operator+(const Vec2 &other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2 &other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(double s) const { return Vec2(x * s, y * s); }
    Vec2 operator/(double s) const { return Vec2(x / s, y / s); }
    Vec2 operator-() const { return Vec2(-x, -y); }

    Vec2 &operator+=(const Vec2 &other) { x += other.x; y += other.y; return *this; }
    Vec2 &operator-=(const Vec2 &other) { x -= other.x; y -= other.y; return *this; }

    bool operator==(const Vec2 &other) const {
        return std::fabs(x - other.x) < EPSILON && std::fabs(y - other.y) < EPSILON;
    }
    bool operator!=(const Vec2 &other) const { return !(*this == other); }

    Vec2 floordiv(double s) const {
        return Vec2(std::floor(x / s), std::floor(y / s));
    }

    double length() const { return std::sqrt(x * x + y * y); }

    static Vec2 slerp(const Vec2 &self, const Vec2 &other, double t);
};

inline Vec2 Vec2::slerp(const Vec2 &self, const Vec2 &other, double t) {
    double length1 = self.length();
    double length2 = other.length();
    if (length1 < EPSILON || length2 < EPSILON) {
        throw std::invalid_argument("can't use slerp with zero vector");
    }
    if (t < -1.0 || t > 1.0) {
        throw std::invalid_argument("argument 2 must be in the range [-1, 1]");
    }

    double tmp = (self.x * other.x + self.y * other.y) / (length1 * length2);
    tmp = (tmp < -1.0 ? -1.0 : (tmp > 1.0 ? 1.0 : tmp));

    double angle;
    if (t < 0.0) {
        angle = std::acos(tmp) - 2.0 * M_PI;
        t = -t;
    } else {
        angle = std::acos(tmp);
    }

    if (angle < EPSILON || std::fabs(angle - 2.0 * M_PI) < EPSILON) {
        return Vec2(self.x * (1.0 - t) + other.x * t,
                    self.y * (1.0 - t) + other.y * t);
    }
    if (std::fabs(angle - M_PI) < EPSILON) {
        throw std::invalid_argument("SLERP with 180 degrees is undefined.");
    }

    double f0 = ((length2 - length1) * t + length1) / std::sin(angle);
    double f1 = std::sin(angle * (1.0 - t)) / length1;
    double f2 = std::sin(angle * t) / length2;

    return Vec2((self.x * f1 + other.x * f2) * f0,
                (self.y * f1 + other.y * f2) * f0);
}

#endif
