#include "engine/Animation.h"

Animation::Animation(const std::vector<double> &frames, int delay, bool loop) {
    this->frames = frames;
    this->delay = delay;
    this->loop = loop;
    this->play = false;
    this->update_func = nullptr;
    this->current_frame = 0;
    this->current_delay = 0;
}

void Animation::update() {
    current_delay += 1;
    if (current_delay >= delay && play) {
        current_delay = 0;
        current_frame += 1;
        if (update_func) {
            update_func();
        } else {
            on_update();
        }
        if (current_frame >= (int)frames.size() && loop) {
            current_frame = 0;
        } else if (current_frame >= (int)frames.size() && !loop) {
            play = false;
        }
    }
}

void Animation::on_update() {}

void Animation::play_animation() {
    current_frame = 0;
    play = true;
}

void Animation::play_animation_at(int frame) {
    current_frame = frame;
    play = true;
}

double Animation::get_current_frame() {
    return frames.at(current_frame);
}

bool Animation::is_playing() {
    return play;
}

bool Animation::ended() {
    return current_frame == (int)frames.size() - 1;
}

void Animation::set_on_update(const std::function<void()> &func) {
    update_func = func;
}
