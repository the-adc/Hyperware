#include "ui/ui_element.h"

#include "ui/ui_manager.h"

namespace ui {

using engine::Rect;

UiElement::UiElement(
	const Rect& relative_rect, UiManager* manager, UiElement* container) :
	relative_rect_(relative_rect),
	manager_(manager),
	container_(container),
	layer_(0),
	alive_(true) {
	if (container) {
		layer_ = container->layer_;
	}
	if (manager) {
		manager->RegisterElement(this);
	}
}

Rect UiElement::ClientRect() const { return AbsoluteRect(); }

Rect UiElement::AbsoluteRect() const {
	if (container_) {
		Rect base = container_->ClientRect();
		return Rect(base.x + relative_rect_.x, base.y + relative_rect_.y,
			relative_rect_.w, relative_rect_.h);
	}
	return relative_rect_;
}

void UiElement::Kill() { alive_ = false; }

}
