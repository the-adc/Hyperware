#include "engine/audio.h"

#include <SFML/Audio.hpp>

#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

namespace {

constexpr std::size_t kMaxChannels = 32;

// one playing (or finished) instance of a sound
struct Channel {
	explicit Channel(const sf::SoundBuffer& buffer) : sound(buffer) {}

	sf::Sound sound;
	audio::SoundId id = audio::kNoSound;
};

bool available = false;
// null for a sound that didn't load, so the ids still line up
std::vector<std::unique_ptr<sf::SoundBuffer>> buffers;
std::vector<std::unique_ptr<Channel>> channels;

bool ValidId(audio::SoundId id) {
	return id >= 0 && id < static_cast<audio::SoundId>(buffers.size()) &&
		buffers[id];
}

// a channel that finished playing, or a new one if there is room
Channel* FreeChannel(const sf::SoundBuffer& buffer) {
	for (std::unique_ptr<Channel>& channel : channels) {
		if (channel->sound.getStatus() == sf::Sound::Status::Stopped) {
			channel->sound.setBuffer(buffer);
			return channel.get();
		}
	}
	if (channels.size() < kMaxChannels) {
		channels.push_back(std::make_unique<Channel>(buffer));
		return channels.back().get();
	}
	return nullptr;
}

}  // namespace

namespace audio {

bool Init() {
	available = sf::PlaybackDevice::getDefaultDevice().has_value();
	if (!available) {
		std::cerr << "No audio device, the game will be silent" << std::endl;
	}
	return available;
}

void Shutdown() {
	// the sounds have to go before the buffers they use
	channels.clear();
	buffers.clear();
	available = false;
}

bool Available() { return available; }

SoundId Load(const std::string& root, const std::string& relative_path) {
	SoundId id = static_cast<SoundId>(buffers.size());
	std::unique_ptr<sf::SoundBuffer> buffer;
	if (available) {
		std::filesystem::path path =
			std::filesystem::path(root) / relative_path;
		buffer = std::make_unique<sf::SoundBuffer>();
		if (!buffer->loadFromFile(path)) {
			std::cerr << "Couldn't load the sound " << path << std::endl;
			buffer.reset();
		}
	}
	buffers.push_back(std::move(buffer));
	return id;
}

void Play(SoundId id, bool loop) {
	if (!available || !ValidId(id)) {
		return;
	}
	Channel* channel = FreeChannel(*buffers[id]);
	if (!channel) {
		return;  // all busy
	}
	channel->id = id;
	channel->sound.setLooping(loop);
	channel->sound.play();
}

bool IsPlaying(SoundId id) {
	for (const std::unique_ptr<Channel>& channel : channels) {
		bool playing = channel->sound.getStatus() == sf::Sound::Status::Playing;
		if (playing && channel->id == id) {
			return true;
		}
	}
	return false;
}

void Stop(SoundId id) {
	for (std::unique_ptr<Channel>& channel : channels) {
		if (channel->id == id) {
			channel->sound.stop();
		}
	}
}

}  // namespace audio
