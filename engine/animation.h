#ifndef HYPER_ENGINE_ANIMATION_H_
#define HYPER_ENGINE_ANIMATION_H_

#include <functional>
#include <vector>

namespace engine {

// A class to represent an animation.
//
// Author: Anas Tarek Qumhiyeh
class Animation {
public:
	// Author: Anas Tarek Qumhiyeh
	//
	// Constructs all the necessary attributes for the Animation object.
	//
	// Parameters
	// ----------
	//     frames : list
	//         A list of frames to animate.
	//     delay : int
	//         The delay between each frame.
	//     loop : bool, optional
	//         A flag indicating whether the animation should loop (default is
	//         False).
	Animation(const std::vector<double>& frames, int delay, bool loop = false);

	// Author: Anas Tarek Qumhiyeh
	//
	// Updates the animation.
	void Update();

	// Author: Anas Tarek Qumhiyeh
	//
	// A function to call when updating the animation.
	void OnUpdate();

	// Author: Anas Tarek Qumhiyeh
	//
	// Plays the animation.
	void PlayAnimation();

	// Author: Anas Tarek Qumhiyeh
	//
	// Plays the animation at a specific frame.
	//
	// Parameters
	// ----------
	//     frame : int
	//         The frame to play the animation at.
	void PlayAnimationAt(int frame);

	// Author: Anas Tarek Qumhiyeh
	//
	// Returns the current frame of the animation.
	//
	// Returns
	// -------
	//     int
	//         The current frame of the animation.
	double CurrentFrame() const;

	// Author: Anas Tarek Qumhiyeh
	//
	// Checks if the animation is playing.
	//
	// Returns
	// -------
	//     bool
	//         True if the animation is playing, False otherwise.
	bool is_playing() const { return play_; }

	// Author: Anas Tarek Qumhiyeh
	//
	// Checks if the animation has ended.
	//
	// Returns
	// -------
	//     bool
	//         True if the animation has ended, False otherwise.
	bool Ended() const;

	// Author: Anas Tarek Qumhiyeh
	//
	// Sets the function to call when updating the animation.
	//
	// Parameters
	// ----------
	//     func : function
	//         The function to call when updating the animation.
	void SetOnUpdate(const std::function<void()>& func);

	const std::vector<double>& frames() const { return frames_; }
	int delay() const { return delay_; }
	bool loop() const { return loop_; }
	int current_frame_index() const { return current_frame_; }
	int current_delay() const { return current_delay_; }

private:
	std::vector<double> frames_;
	int delay_;
	bool loop_;
	bool play_;
	std::function<void()> update_func_;
	int current_frame_;
	int current_delay_;
};

}  // namespace engine

#endif  // HYPER_ENGINE_ANIMATION_H_
