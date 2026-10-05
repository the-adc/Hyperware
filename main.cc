#include "engine/positions/world.h"
#include "game/game.h"

// Create a game instance and run it
int main() {
	if (!game::Init()) {
		return 1;
	}

	// 60 steps a second until the game is quit
	game::GetWorld()->Run();

	game::Shutdown();
	return 0;
}
