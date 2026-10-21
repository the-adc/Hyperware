#include "ui/Theme.h"

#include <cstdlib>
#include <fstream>

Theme::Theme() {}

void Theme::load(const std::string &path) {
    std::ifstream file(graphics::resolve_path(path));
    if (!file.is_open()) {
        data = nlohmann::json::object();
        return;
    }
    try {
        file >> data;
    } catch (...) {
        data = nlohmann::json::object();
    }
}

Color Theme::parse_colour(const std::string &text, const Color &fallback) {
    if (text.empty()) {
        return fallback;
    }
    if (text[0] == '#') {
        std::string hex = text.substr(1);
        if (hex.size() == 6 || hex.size() == 8) {
            long value = std::strtol(hex.c_str(), nullptr, 16);
            if (hex.size() == 6) {
                return Color((int)((value >> 16) & 0xFF), (int)((value >> 8) & 0xFF),
                             (int)(value & 0xFF));
            }
            return Color((int)((value >> 24) & 0xFF), (int)((value >> 16) & 0xFF),
                         (int)((value >> 8) & 0xFF), (int)(value & 0xFF));
        }
        return fallback;
    }

    std::string lowered = graphics::string_lower(text);
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

Color Theme::colour(const std::string &block, const std::string &key,
                    const Color &fallback) const {
    if (data.contains(block) && data[block].contains("colours") &&
        data[block]["colours"].contains(key)) {
        return parse_colour(data[block]["colours"][key].get<std::string>(), fallback);
    }
    return fallback;
}

std::string Theme::misc(const std::string &block, const std::string &key,
                        const std::string &fallback) const {
    if (data.contains(block) && data[block].contains("misc") &&
        data[block]["misc"].contains(key)) {
        return data[block]["misc"][key].get<std::string>();
    }
    return fallback;
}

int Theme::misc_int(const std::string &block, const std::string &key, int fallback) const {
    std::string text = misc(block, key, "");
    if (text.empty()) {
        return fallback;
    }
    return std::atoi(text.c_str());
}

std::string Theme::font_path(const std::string &block, const std::string &fallback) const {
    if (data.contains(block) && data[block].contains("font") &&
        data[block]["font"].contains("regular_path")) {
        return data[block]["font"]["regular_path"].get<std::string>();
    }
    return fallback;
}

int Theme::font_size(const std::string &block, int fallback) const {
    if (data.contains(block) && data[block].contains("font") &&
        data[block]["font"].contains("size")) {
        return std::atoi(data[block]["font"]["size"].get<std::string>().c_str());
    }
    return fallback;
}
