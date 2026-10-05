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

find_package(SFML 3 COMPONENTS Graphics Audio CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# engine/ is a library, game/ is the game
set(ENGINE_SOURCES
    engine/actions/action_list.cc
    engine/actions/destroy_self_action.cc
    engine/actions/do_nothing_action.cc
    engine/actions/sequence_action.cc
    engine/actions/set_motion_action.cc
    engine/actors/actor.cc
    engine/app.cc
    engine/assets.cc
    engine/audio.cc
    engine/capabilities/capability_set.cc
    engine/display/display.cc
    engine/input.cc
    engine/positions/game_map.cc
    engine/positions/world.cc
    engine/random.cc
    engine/utils.cc)

set(GAME_SOURCES
    game/game.cc
    game/objects.cc
    game/resources.cc

    game/objects/player.cc)

file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS "engine/*.h")
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS "game/*.h")

add_library(engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

add_executable(MyGame main.cc ${GAME_SOURCES} ${GAME_HEADERS})

# so includes can be "engine/foo.h"
target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(engine PUBLIC
    SFML::Graphics
    SFML::Audio
    nlohmann_json::nlohmann_json)

target_link_libraries(MyGame PRIVATE engine)

if(MSVC)
    foreach(target engine MyGame)
        target_compile_options(${target} PRIVATE /utf-8 /W4)
        target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
    endforeach()
endif()

# the exe and assets/ end up together in bin/
set_target_properties(MyGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                  "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG            "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE          "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO   "${CMAKE_BINARY_DIR}/bin"
    VS_DEBUGGER_WORKING_DIRECTORY             "${CMAKE_BINARY_DIR}/bin")

add_custom_command(TARGET MyGame POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/bin/assets")

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT MyGame)

# folders in the VS solution explorer
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    main.cc ${ENGINE_SOURCES} ${ENGINE_HEADERS} ${GAME_SOURCES} ${GAME_HEADERS})
