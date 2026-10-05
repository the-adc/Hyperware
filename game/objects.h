#ifndef HYPER_GAME_OBJECTS_H_
#define HYPER_GAME_OBJECTS_H_

namespace engine {
class World;
}  // namespace engine

namespace game {

enum ObjectKind { kPlayer = 0, kObjectCount };

void RegisterCollisions(engine::World* world);

}  // namespace game

#endif  // HYPER_GAME_OBJECTS_H_
