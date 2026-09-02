#include "engine/animation.h"

#include <cstddef>

namespace engine {

Animation::Animation(const std::vector<double>& frames, int delay, bool loop) :
	frames_(frames),
	delay_(delay),
	loop_(loop),
	play_(false),
	update_func_(nullptr),
	current_frame_(0),
	current_delay_(0) {}

void Animation::Update() {
	current_delay_ += 1;
	if (current_delay_ >= delay_ && play_) {
		current_delay_ = 0;
		current_frame_ += 1;
		if (update_func_) {
			update_func_();
		} else {
			OnUpdate();
		}
		if (current_frame_ >= static_cast<int>(frames_.size()) && loop_) {
			current_frame_ = 0;
		} else if (current_frame_ >= static_cast<int>(frames_.size()) &&
			!loop_) {
			play_ = false;
		}
	}
}

void Animation::OnUpdate() {}

void Animation::PlayAnimation() {
	current_frame_ = 0;
	play_ = true;
}

void Animation::PlayAnimationAt(int frame) {
	current_frame_ = frame;
	play_ = true;
}

double Animation::CurrentFrame() const {
	if (frames_.empty()) {
		return 0.0;
	}
	int last = static_cast<int>(frames_.size()) - 1;
	int index = current_frame_ < 0
		? 0
		: (current_frame_ > last ? last : current_frame_);
	return frames_[static_cast<std::size_t>(index)];
}

bool Animation::Ended() const {
	return current_frame_ == static_cast<int>(frames_.size()) - 1;
}

void Animation::SetOnUpdate(const std::function<void()>& func) {
	update_func_ = func;
}

}
