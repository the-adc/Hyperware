#ifndef HYPER_ENGINE_CAPABILITIES_CAPABILITY_H_
#define HYPER_ENGINE_CAPABILITIES_CAPABILITY_H_

#include <cstddef>
#include <functional>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

namespace engine {

// Wraps a value of any enum so different enums can share one set
class Capability {
public:
	template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
	Capability(E value)  // NOLINT(google-explicit-constructor)
		: type_(typeid(E)), value_(static_cast<int>(value)) {}

	bool operator==(const Capability& other) const {
		return type_ == other.type_ && value_ == other.value_;
	}
	bool operator!=(const Capability& other) const { return !(*this == other); }

	const std::type_index& type() const { return type_; }
	int value() const { return value_; }

	template <typename E>
	bool IsOfType() const {
		return type_ == std::type_index(typeid(E));
	}

private:
	std::type_index type_;
	int value_;
};

}  // namespace engine

namespace std {

template <>
struct hash<engine::Capability> {
	size_t operator()(const engine::Capability& capability) const {
		size_t seed = capability.type().hash_code();
		seed ^= std::hash<int>()(capability.value()) + 0x9e3779b9 + (seed << 6) +
			(seed >> 2);
		return seed;
	}
};

}  // namespace std

#endif  // HYPER_ENGINE_CAPABILITIES_CAPABILITY_H_
