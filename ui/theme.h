#ifndef UI_THEME_H_
#define UI_THEME_H_

#include <nlohmann/json.hpp>
#include <string>

#include "engine/graphics.h"

namespace ui {

class Theme {
public:
	Theme() = default;

	void Load(const std::string& path);

	engine::Color Colour(const std::string& block, const std::string& key,
		const engine::Color& fallback) const;
	std::string Misc(const std::string& block, const std::string& key,
		const std::string& fallback) const;
	int MiscInt(
		const std::string& block, const std::string& key, int fallback) const;
	std::string FontPath(
		const std::string& block, const std::string& fallback) const;
	int FontSize(const std::string& block, int fallback) const;

	static engine::Color ParseColour(
		const std::string& text, const engine::Color& fallback);

private:
	nlohmann::json data_;
};

}

#endif
