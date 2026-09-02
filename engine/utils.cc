#include "engine/utils.h"

#include <SDL_mixer.h>

#include <cmath>
#include <map>

#include "engine/graphics.h"
#include "engine/vec2.h"

namespace engine {

namespace {

std::map<std::string, Mix_Chunk*>& SoundCache() {
	static auto* cache = new std::map<std::string, Mix_Chunk*>();
	return *cache;
}

Mix_Music* g_music = nullptr;

}

void MixerInit() {
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(32);
}

void MixerQuit() {
	for (std::map<std::string, Mix_Chunk*>::iterator it = SoundCache().begin();
		it != SoundCache().end(); ++it) {
		Mix_FreeChunk(it->second);
	}
	SoundCache().clear();
	if (g_music) {
		Mix_HaltMusic();
		Mix_FreeMusic(g_music);
		g_music = nullptr;
	}
	Mix_CloseAudio();
}

double LengthDirX(double dist, double angle) {
	double radian_angle = angle * kPi / 180;
	return dist * std::cos(radian_angle);
}

double LengthDirY(double dist, double angle) {
	double radian_angle = angle * kPi / 180;
	return dist * -std::sin(radian_angle);
}

void PlaySoundEffect(const std::string& sound_file) {
	std::string path = "assets/sounds/" + sound_file + ".ogg";
	std::map<std::string, Mix_Chunk*>::iterator found = SoundCache().find(path);
	Mix_Chunk* snd;
	if (found != SoundCache().end()) {
		snd = found->second;
	} else {
		snd = Mix_LoadWAV(graphics::ResolvePath(path).c_str());
		SoundCache()[path] = snd;
	}
	if (snd) {
		Mix_PlayChannel(-1, snd, 0);
	}
}

void PlayMusic(const std::string& music_file) {
	if (g_music) {
		Mix_HaltMusic();
		Mix_FreeMusic(g_music);
		g_music = nullptr;
	}
	g_music = Mix_LoadMUS(
		graphics::ResolvePath("assets/sounds/" + music_file + ".ogg").c_str());
	if (g_music) {
		Mix_PlayMusic(g_music, -1);
	}
}

void ToggleMusic() {
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	} else {
		PlayMusic("musBackground");
	}
}

}
