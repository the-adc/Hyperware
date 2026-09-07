#ifndef HYPER_ENGINE_CAPABILITIES_CAPABILITY_SET_H_
#define HYPER_ENGINE_CAPABILITIES_CAPABILITY_SET_H_

#include <unordered_set>
#include <vector>

#include "engine/capabilities/capability.h"

namespace engine {

class CapabilitySet {
public:
	virtual ~CapabilitySet() = default;

	bool HasCapability(const Capability& capability) const;
	void AddCapability(const Capability& capability);
	void RemoveCapability(const Capability& capability);

	std::vector<Capability> CapabilitiesList() const;

	template <typename E>
	std::vector<E> FindCapabilitiesByType() const {
		std::vector<E> found;
		for (const Capability& capability : capabilities_) {
			if (capability.IsOfType<E>()) {
				found.push_back(static_cast<E>(capability.value()));
			}
		}
		return found;
	}

private:
	std::unordered_set<Capability> capabilities_;
};

}  // namespace engine

#endif  // HYPER_ENGINE_CAPABILITIES_CAPABILITY_SET_H_
