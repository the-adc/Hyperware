#ifndef HYPER_ENGINE_AUDIO_H_
#define HYPER_ENGINE_AUDIO_H_

#include <string>

namespace audio {

using SoundId = int;
constexpr SoundId kNoSound = -1;

bool Init();
void Shutdown();
bool Available();


SoundId Load(const std::string& root, const std::string& relative_path);

void Play(SoundId id, bool loop);
bool IsPlaying(SoundId id);
void Stop(SoundId id);

}  // namespace audio

#endif  // HYPER_ENGINE_AUDIO_H_
