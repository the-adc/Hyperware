#include "engine/utils.h"

#include <SDL_mixer.h>

#include <cmath>
#include <map>

#include "engine/graphics.h"

namespace engine {

namespace {

std::map<std::string, Mix_Chunk*>& SoundCache() {
	static auto* cache = new std::map<std::string, Mix_Chunk*>();
	return *cache;
}

Mix_Music* g_music = nullptr;

int ToChannel(double value) {
	int channel = static_cast<int>(value);
	if (channel < 0) {
		return 0;
	}
	if (channel > 255) {
		return 255;
	}
	return channel;
}

}  // namespace

void Util::MixerInit() {
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(32);
}

void Util::MixerQuit() {
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

double Util::LengthDirX(double dist, double angle) {
	double radian_angle = angle * kPi / 180;
	return dist * std::cos(radian_angle);
}

double Util::LengthDirY(double dist, double angle) {
	double radian_angle = angle * kPi / 180;
	return dist * -std::sin(radian_angle);
}

Color Util::MakeColorRgb(double r, double g, double b) {
	return Color(ToChannel(r), ToChannel(g), ToChannel(b));
}

Color Util::MergeColor(const Color& first, const Color& second,
	double amount) {
	return Color(ToChannel(first.r + (second.r - first.r) * amount),
		ToChannel(first.g + (second.g - first.g) * amount),
		ToChannel(first.b + (second.b - first.b) * amount));
}

double Util::PointDirection(double x1, double y1, double x2, double y2) {
	double degrees = std::atan2(y1 - y2, x2 - x1) * 180.0 / kPi;
	if (degrees < 0.0) {
		degrees += 360.0;
	}
	return degrees;
}

double Util::PointDistance(double x1, double y1, double x2, double y2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	return std::sqrt(dx * dx + dy * dy);
}

double Util::Lerp(double from, double to, double amount) {
	return from + (to - from) * amount;
}

double Util::Clamp(double value, double low, double high) {
	if (value < low) {
		return low;
	}
	if (value > high) {
		return high;
	}
	return value;
}

void Util::PlaySFX(const std::string& sound_file) {
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

void Util::PlayMusic(const std::string& music_file) {
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

void Util::ToggleMusic() {
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	} else {
		PlayMusic("musBackground");
	}
}

}  // namespace engine
