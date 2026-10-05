#ifndef HYPER_GAME_RESOURCES_H_
#define HYPER_GAME_RESOURCES_H_

#include <string>

#include "engine/assets.h"
#include "engine/audio.h"
#include "engine/display/display.h"

/* The sprites, sounds and font of the game. Declare an id here for each one,
    define it in resources.cc and look it up in Load().

    extern assets::SpriteId spr_player;
    extern audio::SoundId snd_shoot; */
namespace res {

bool Load(engine::Display* display, const std::string& assets_root);

}  // namespace res

#endif  // HYPER_GAME_RESOURCES_H_
