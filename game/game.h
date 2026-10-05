#ifndef HYPER_GAME_GAME_H_
#define HYPER_GAME_GAME_H_

#include "engine/display/display.h"
#include "engine/positions/game_map.h"
#include "engine/positions/world.h"

// this is where the maps are built and the engine starts when the window is set up
namespace game {

// both are valid once Init has run
engine::World* GetWorld();
engine::Display* GetDisplay();

// main GameMap that you start in
engine::GameMap* MainRoom();

bool Init();
void Shutdown();

void Quit();

}  // namespace game

#endif  // HYPER_GAME_GAME_H_
