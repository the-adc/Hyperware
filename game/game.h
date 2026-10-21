#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <memory>

#include "engine/Clock.h"
#include "engine/Graphics.h"
#include "engine/Vec2.h"
#include "game/Scene.h"

/*
Author: Yash Mahmud

A class used to represent a Game.

...

Attributes
----------
running : bool
    A flag indicating whether the game is running.
SCREEN_SIZE : Vector2
    The size of the game screen.
SCREEN_CENTER : Vector2
    The center of the game screen.
screen : Surface
    The game screen.
board : Board
    The game board.
finished : bool
    A flag indicating whether the game is finished.
clock : Clock
    The game clock.
winner_text : Text
    The text displaying the winner of the game.

Methods
-------
__init__():
    Constructs all the necessary attributes for the game object.
draw():
    Draws the game on the screen.
update():
    Updates the state of the game.
events():
    Handles the game events.
run():
    Runs the game.
change_scene(scene):
    Changes the current scene to the provided scene.
quit():
    Quits the game.
*/
class Game {
public:
    /*
    Author: Yash Mahmud

    Constructs all the necessary attributes for the game object.
    */
    Game();

    ~Game();

    /*
    Author: Yash Mahmud

    Draws the game on the screen.
    */
    void draw();

    /*
    Author: Yash Mahmud

    Updates the state of the game.
    */
    void update();

    /*
    Author: Yash Mahmud

    Handles the game events.
    */
    void events();

    /*
    Author: Yash Mahmud

    Runs the game.
    */
    void run();

    /*
    Author: Yash Mahmud

    Changes the current scene to the provided scene.
    */
    void change_scene(std::unique_ptr<Scene> scene);

    /*
    Author: Yash Mahmud

    Quits the game.
    */
    void quit();

    bool running;
    Vec2 SCREEN_SIZE;
    Vec2 SCREEN_CENTER;
    Surface *screen;
    std::unique_ptr<Scene> current_scene;
    std::unique_ptr<Scene> previous_scene;
    Clock clock;
};

#endif
