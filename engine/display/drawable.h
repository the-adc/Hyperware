#ifndef HYPER_ENGINE_DISPLAY_DRAWABLE_H_
#define HYPER_ENGINE_DISPLAY_DRAWABLE_H_

namespace engine {

class Display;

// Anything that can draw itself on the Display. Highest depth goes first.
class Drawable {
public:
	virtual ~Drawable() = default;

	virtual void Draw(Display* display) = 0;
	virtual int depth() const = 0;
	virtual bool visible() const { return true; }
};

}  // namespace engine

#endif  // HYPER_ENGINE_DISPLAY_DRAWABLE_H_
