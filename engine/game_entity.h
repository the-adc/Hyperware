#ifndef HYPER_ENGINE_GAME_ENTITY_H_
#define HYPER_ENGINE_GAME_ENTITY_H_

#include <vector>

#include "engine/capabilities/capability.h"
#include "engine/capabilities/capability_set.h"

namespace engine {

// Instance of an object in the game world.
class GameEntity {
public:
	virtual ~GameEntity() = default;

	void AddCapability(const Capability& capability) {
		capability_set_.AddCapability(capability);
	}
	void RemoveCapability(const Capability& capability) {
		capability_set_.RemoveCapability(capability);
	}

	virtual bool HasCapability(const Capability& capability) const {
		return capability_set_.HasCapability(capability);
	}

	std::vector<Capability> CapabilitiesList() const {
		return capability_set_.CapabilitiesList();
	}

	template <typename E>
	std::vector<E> FindCapabilitiesByType() const {
		return capability_set_.FindCapabilitiesByType<E>();
	}

protected:
	const CapabilitySet& capability_set() const { return capability_set_; }

private:
	CapabilitySet capability_set_;
};

}  // namespace engine

#endif  // HYPER_ENGINE_GAME_ENTITY_H_
