#ifndef HYPER_ENGINE_COLOR_H_
#define HYPER_ENGINE_COLOR_H_

#include <SFML/Graphics/Color.hpp>

namespace engine {

using Color = sf::Color;

// these colors are based off the color constants from GameMaker
// NOLINTBEGIN(readability-identifier-naming)
inline constexpr Color C_AQUA(0, 255, 255);
inline constexpr Color C_BLACK(0, 0, 0);
inline constexpr Color C_BLUE(0, 0, 255);
inline constexpr Color C_DKGRAY(64, 64, 64);
inline constexpr Color C_FUCHSIA(255, 0, 255);
inline constexpr Color C_GRAY(128, 128, 128);
inline constexpr Color C_GREEN(0, 128, 0);
inline constexpr Color C_LIME(0, 255, 0);
inline constexpr Color C_LTGRAY(192, 192, 192);
inline constexpr Color C_MAROON(128, 0, 0);
inline constexpr Color C_NAVY(0, 0, 128);
inline constexpr Color C_OLIVE(128, 128, 0);
inline constexpr Color C_ORANGE(255, 160, 64);
inline constexpr Color C_PURPLE(128, 0, 128);
inline constexpr Color C_RED(255, 0, 0);
inline constexpr Color C_TEAL(0, 128, 128);
inline constexpr Color C_WHITE(255, 255, 255);
inline constexpr Color C_YELLOW(255, 255, 0);
// NOLINTEND(readability-identifier-naming)

}  // namespace engine

#endif  // HYPER_ENGINE_COLOR_H_
