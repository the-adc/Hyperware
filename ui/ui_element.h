#ifndef UI_UI_ELEMENT_H_
#define UI_UI_ELEMENT_H_

#include "engine/events.h"
#include "engine/graphics.h"

namespace ui {

class UiManager;

class UiElement {
public:
	UiElement(const engine::Rect& relative_rect, UiManager* manager,
		UiElement* container);

	UiElement(const UiElement&) = delete;
	UiElement& operator=(const UiElement&) = delete;

	virtual ~UiElement() = default;

	virtual bool ProcessEvent(const engine::Event& /*event*/) { return false; }
	virtual bool ProcessOverlayEvent(const engine::Event& /*event*/) {
		return false;
	}
	virtual void Update(double /*time_seconds*/) {}
	virtual void Draw(engine::Surface& /*surface*/) {}
	virtual void DrawOverlay(engine::Surface& /*surface*/) {}

	virtual engine::Rect ClientRect() const;
	engine::Rect AbsoluteRect() const;
	void Kill();

	const engine::Rect& relative_rect() const { return relative_rect_; }
	UiManager* manager() const { return manager_; }
	UiElement* container() const { return container_; }
	int layer() const { return layer_; }
	void set_layer(int layer) { layer_ = layer; }
	bool alive() const { return alive_; }

private:
	engine::Rect relative_rect_;
	UiManager* manager_;
	UiElement* container_;
	int layer_;
	bool alive_;
};

}

#endif
