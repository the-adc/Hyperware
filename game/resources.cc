#include "game/resources.h"

#include "engine/display/display.h"

namespace res {

/*  This is how you load sprites:
    spr_player = assets::FindSprite("player");

    Sounds and the font get loaded here:
    snd_shoot = audio::Load(assets_root, "sounds/shoot.wav");

    if (!assets::LoadFont(assets_root, "fonts/font.ttf")) {
     return false;
    }
    display->SetFont(&assets::GameFont()); */
bool Load(engine::Display* /*display*/, const std::string& /*assets_root*/) {
	return true;
}

}  // namespace res
