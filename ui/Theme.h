#ifndef UI_THEME_H
#define UI_THEME_H

#include <nlohmann/json.hpp>

#include <string>

#include "engine/Graphics.h"

class Theme {
public:
    Theme();

    void load(const std::string &path);

    Color colour(const std::string &block, const std::string &key, const Color &fallback) const;
    std::string misc(const std::string &block, const std::string &key,
                     const std::string &fallback) const;
    int misc_int(const std::string &block, const std::string &key, int fallback) const;
    std::string font_path(const std::string &block, const std::string &fallback) const;
    int font_size(const std::string &block, int fallback) const;

    static Color parse_colour(const std::string &text, const Color &fallback);

private:
    nlohmann::json data;
};

#endif
