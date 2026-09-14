#ifndef HYPER_ENGINE_ASSETS_H_
#define HYPER_ENGINE_ASSETS_H_

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace assets {

using SpriteId = int;
constexpr SpriteId kNoSprite = -1;

enum class MaskKind { kRectangle, kPrecise, kPrecisePerFrame };

struct SpriteFrame {
	// where the frame is on the sprite sheet
	sf::IntRect rect;
	// only for precise masks
	std::vector<std::uint8_t> mask;
};

// this takes from an Aseprite tag
struct Tag {
	std::string name;
	int from = 0;
	int to = 0;
};

struct Sprite {
	std::string name;
	sf::Texture texture;
	int width = 0;
	int height = 0;
	int xorigin = 0;
	int yorigin = 0;
	// collision box, from the top left of the image
	int bbox_left = 0;
	int bbox_top = 0;
	int bbox_right = 0;
	int bbox_bottom = 0;
	MaskKind mask = MaskKind::kRectangle;
	double fps = 30.0;
	std::vector<SpriteFrame> frames;
	std::vector<Tag> tags;

	int FrameCount() const { return static_cast<int>(frames.size()); }
	bool IsPrecise() const {
		return mask == MaskKind::kPrecise || mask == MaskKind::kPrecisePerFrame;
	}
	const Tag* FindTag(const std::string& tag_name) const;
};

// Loads every sprite in root/sprites. A sprite is name.png and name.json,
bool Load(const std::string& root);
bool LoadFont(const std::string& root, const std::string& relative_path);
void Unload();

SpriteId FindSprite(const std::string& name);
const Sprite& GetSprite(SpriteId id);

const sf::Font& GameFont();

}  // namespace assets

#endif  // HYPER_ENGINE_ASSETS_H_
