#include "engine/assets.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

namespace {

namespace fs = std::filesystem;

std::vector<assets::Sprite> sprites;
sf::Font font;

bool ReadJson(const fs::path& path, nlohmann::json* json) {
	std::ifstream file(path);
	if (!file) {
		std::cerr << "Couldn't open " << path << std::endl;
		return false;
	}
	try {
		file >> *json;
	} catch (const std::exception& error) {
		std::cerr << "Bad json in " << path << ": " << error.what() << std::endl;
		return false;
	}
	return true;
}

void ReadFrames(const nlohmann::json& json, assets::Sprite* sprite) {
	for (const nlohmann::json& frame : json["frames"]) {
		const nlohmann::json& r = frame["frame"];
		assets::SpriteFrame sprite_frame;
		sprite_frame.rect = sf::IntRect({r["x"].get<int>(), r["y"].get<int>()},
			{r["w"].get<int>(), r["h"].get<int>()});
		sprite->frames.push_back(sprite_frame);
	}
	if (sprite->frames.empty()) {
		return;
	}

	const nlohmann::json& first = json["frames"][0];
	sprite->width = first["sourceSize"]["w"].get<int>();
	sprite->height = first["sourceSize"]["h"].get<int>();
	sprite->bbox_right = sprite->width - 1;
	sprite->bbox_bottom = sprite->height - 1;

	// Aseprite has a duration for every frame, GameMaker only one speed
	int duration = first["duration"].get<int>();
	if (duration > 0) {
		sprite->fps = 1000.0 / duration;
	}
}

void ReadTags(const nlohmann::json& json, assets::Sprite* sprite) {
	for (const nlohmann::json& tag : json["meta"]["frameTags"]) {
		sprite->tags.push_back({tag["name"].get<std::string>(),
			tag["from"].get<int>(), tag["to"].get<int>()});
	}
}

bool LoadSprite(const fs::path& json_path, assets::Sprite* sprite) {
	nlohmann::json json;
	if (!ReadJson(json_path, &json)) {
		return false;
	}

	sprite->name = json_path.stem().string();
	ReadFrames(json, sprite);
	if (sprite->frames.empty()) {
		std::cerr << sprite->name << " doesn't have any frames" << std::endl;
		return false;
	}
	ReadTags(json, sprite);

	fs::path image =
		json_path.parent_path() / json["meta"]["image"].get<std::string>();
	if (!sprite->texture.loadFromFile(image)) {
		std::cerr << "Couldn't load the image " << image << std::endl;
		return false;
	}
	return true;
}

}  // namespace

namespace assets {

const Tag* Sprite::FindTag(const std::string& tag_name) const {
	for (const Tag& tag : tags) {
		if (tag.name == tag_name) {
			return &tag;
		}
	}
	return nullptr;
}

bool Load(const std::string& root) {
	Unload();

	fs::path folder = fs::path(root) / "sprites";
	if (!fs::exists(folder)) {
		return true;
	}

	std::vector<fs::path> files;
	for (const auto& entry : fs::directory_iterator(folder)) {
		if (entry.path().extension() == ".json") {
			files.push_back(entry.path());
		}
	}
	std::sort(files.begin(), files.end());  // so the ids are always the same

	for (const fs::path& file : files) {
		Sprite sprite;
		if (!LoadSprite(file, &sprite)) {
			return false;
		}
		sprites.push_back(std::move(sprite));
	}
	return true;
}

bool LoadFont(const std::string& root, const std::string& relative_path) {
	fs::path path = fs::path(root) / relative_path;
	if (!font.openFromFile(path)) {
		std::cerr << "Couldn't load the font " << path << std::endl;
		return false;
	}
	return true;
}

void Unload() {
	sprites.clear();
	font = sf::Font();
}

SpriteId FindSprite(const std::string& name) {
	for (std::size_t i = 0; i < sprites.size(); ++i) {
		if (sprites[i].name == name) {
			return static_cast<SpriteId>(i);
		}
	}
	std::cerr << "No sprite called " << name << std::endl;
	return kNoSprite;
}

const Sprite& GetSprite(SpriteId id) {
	static const Sprite empty;
	if (id < 0 || id >= static_cast<SpriteId>(sprites.size())) {
		return empty;
	}
	return sprites[static_cast<std::size_t>(id)];
}

const sf::Font& GameFont() { return font; }

}  // namespace assets
