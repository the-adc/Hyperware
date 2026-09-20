cmake_minimum_required(VERSION 3.21)

# use vcpkg if VCPKG_ROOT is set
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "vcpkg toolchain file")
endif()

project(MyGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SFML 3 COMPONENTS Graphics CONFIG REQUIRED)

# engine/ is a library, game/ is the game
set(ENGINE_SOURCES
    engine/actions/action_list.cc
    engine/actions/destroy_self_action.cc
    engine/actions/do_nothing_action.cc
    engine/actions/sequence_action.cc
    engine/actions/set_motion_action.cc
    engine/actors/actor.cc
    engine/animation.cc
    engine/assets.cc
    engine/button.cc
    engine/capabilities/capability_set.cc
    engine/clock.cc
    engine/display/display.cc
    engine/events.cc
    engine/graphics.cc
    engine/positions/game_map.cc
    engine/positions/world.cc
    engine/random.cc
    engine/sprite.cc
    engine/text.cc
    engine/utils.cc

    ui/draw.cc
    ui/theme.cc
    ui/ui_button.cc
    ui/ui_drop_down_menu.cc
    ui/ui_element.cc
    ui/ui_manager.cc
    ui/ui_text_box.cc
    ui/ui_window.cc)

set(GAME_SOURCES
    game/game.cc)

file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS "engine/*.h" "ui/*.h")
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS "game/*.h")

add_library(engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

add_executable(MyGame main.cc ${GAME_SOURCES} ${GAME_HEADERS})

# so includes can be "engine/foo.h"
target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(engine PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    SFML::Graphics
    nlohmann_json::nlohmann_json)

target_link_libraries(MyGame PRIVATE engine SDL2::SDL2main)

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT MyGame)

# folders in the VS solution explorer
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    main.cc ${ENGINE_SOURCES} ${ENGINE_HEADERS} ${GAME_SOURCES} ${GAME_HEADERS})
