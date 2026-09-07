#include "engine/capabilities/capability_set.h"

namespace engine {

bool CapabilitySet::HasCapability(const Capability& capability) const {
	return capabilities_.count(capability) != 0;
}

void CapabilitySet::AddCapability(const Capability& capability) {
	capabilities_.insert(capability);
}

void CapabilitySet::RemoveCapability(const Capability& capability) {
	capabilities_.erase(capability);
}

std::vector<Capability> CapabilitySet::CapabilitiesList() const {
	return std::vector<Capability>(capabilities_.begin(), capabilities_.end());
}

}  // namespace engine
