#include "ui/theme.h"

#include <cstdlib>
#include <fstream>

#include "engine/graphics.h"

namespace ui {

using engine::Color;

namespace {

const std::string* LookupString(const nlohmann::json& data,
	const std::string& block, const char* group, const std::string& key) {
	if (!data.is_object()) {
		return nullptr;
	}
	nlohmann::json::const_iterator block_it = data.find(block);
	if (block_it == data.end() || !block_it->is_object()) {
		return nullptr;
	}
	nlohmann::json::const_iterator group_it = block_it->find(group);
	if (group_it == block_it->end() || !group_it->is_object()) {
		return nullptr;
	}
	nlohmann::json::const_iterator key_it = group_it->find(key);
	if (key_it == group_it->end() || !key_it->is_string()) {
		return nullptr;
	}
	return &key_it->get_ref<const std::string&>();
}

}  // namespace

void Theme::Load(const std::string& path) {
	std::ifstream file(engine::graphics::ResolvePath(path));
	if (!file.is_open()) {
		data_ = nlohmann::json::object();
		return;
	}
	data_ = nlohmann::json::parse(file, nullptr, false);
	if (data_.is_discarded()) {
		data_ = nlohmann::json::object();
	}
}

Color Theme::ParseColour(const std::string& text, const Color& fallback) {
	if (text.empty()) {
		return fallback;
	}
	if (text[0] == '#') {
		std::string hex = text.substr(1);
		if (hex.size() == 6 || hex.size() == 8) {
			long value = std::strtol(hex.c_str(), nullptr, 16);
			if (hex.size() == 6) {
				return Color(static_cast<int>((value >> 16) & 0xFF),
					static_cast<int>((value >> 8) & 0xFF),
					static_cast<int>(value & 0xFF));
			}
			return Color(static_cast<int>((value >> 24) & 0xFF),
				static_cast<int>((value >> 16) & 0xFF),
				static_cast<int>((value >> 8) & 0xFF),
				static_cast<int>(value & 0xFF));
		}
		return fallback;
	}

	std::string lowered = engine::graphics::StringLower(text);
	if (lowered == "white") {
		return Color(255, 255, 255);
	}
	if (lowered == "black") {
		return Color(0, 0, 0);
	}
	if (lowered == "red") {
		return Color(255, 0, 0);
	}
	return fallback;
}

Color Theme::Colour(const std::string& block, const std::string& key,
	const Color& fallback) const {
	const std::string* value = LookupString(data_, block, "colours", key);
	if (value) {
		return ParseColour(*value, fallback);
	}
	return fallback;
}

std::string Theme::Misc(const std::string& block, const std::string& key,
	const std::string& fallback) const {
	const std::string* value = LookupString(data_, block, "misc", key);
	return value ? *value : fallback;
}

int Theme::MiscInt(
	const std::string& block, const std::string& key, int fallback) const {
	std::string text = Misc(block, key, "");
	if (text.empty()) {
		return fallback;
	}
	return std::atoi(text.c_str());
}

std::string Theme::FontPath(
	const std::string& block, const std::string& fallback) const {
	const std::string* value =
		LookupString(data_, block, "font", "regular_path");
	return value ? *value : fallback;
}

int Theme::FontSize(const std::string& block, int fallback) const {
	const std::string* value = LookupString(data_, block, "font", "size");
	if (value) {
		return std::atoi(value->c_str());
	}
	return fallback;
}

}  // namespace ui
