#include "engine/Utils.h"

#include <SDL_mixer.h>

#include <cmath>
#include <map>

#include "engine/Graphics.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace {

std::map<std::string, Mix_Chunk *> &sound_cache() {
    static std::map<std::string, Mix_Chunk *> cache;
    return cache;
}

Mix_Music *g_music = nullptr;

}

void mixer_init() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(32);
}

void mixer_quit() {
    for (std::map<std::string, Mix_Chunk *>::iterator it = sound_cache().begin();
         it != sound_cache().end(); ++it) {
        Mix_FreeChunk(it->second);
    }
    sound_cache().clear();
    if (g_music) {
        Mix_HaltMusic();
        Mix_FreeMusic(g_music);
        g_music = nullptr;
    }
    Mix_CloseAudio();
}

double lengthdir_x(double dist, double angle) {
    double radian_angle = angle * M_PI / 180;
    return dist * std::cos(radian_angle);
}

double lengthdir_y(double dist, double angle) {
    double radian_angle = angle * M_PI / 180;
    return dist * -std::sin(radian_angle);
}

void play_sound(const std::string &sound_file) {
    std::string path = "assets/sounds/" + sound_file + ".ogg";
    std::map<std::string, Mix_Chunk *>::iterator found = sound_cache().find(path);
    Mix_Chunk *snd;
    if (found != sound_cache().end()) {
        snd = found->second;
    } else {
        snd = Mix_LoadWAV(graphics::resolve_path(path).c_str());
        sound_cache()[path] = snd;
    }
    if (snd) {
        Mix_PlayChannel(-1, snd, 0);
    }
}

void play_music(const std::string &music_file) {
    if (g_music) {
        Mix_HaltMusic();
        Mix_FreeMusic(g_music);
        g_music = nullptr;
    }
    g_music = Mix_LoadMUS(graphics::resolve_path("assets/sounds/" + music_file + ".ogg").c_str());
    if (g_music) {
        Mix_PlayMusic(g_music, -1);
    }
}

void toggle_music() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    } else {
        play_music("musBackground");
    }
}
