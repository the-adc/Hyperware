#ifndef ENGINE_RANDOM_H_
#define ENGINE_RANDOM_H_

#include <algorithm>
#include <cstddef>
#include <random>
#include <vector>

namespace engine {

std::mt19937& Rng();

int RandInt(int a, int b);

template <typename T>
void Shuffle(std::vector<T>& values) {
	std::shuffle(values.begin(), values.end(), Rng());
}

template <typename T>
T& Choice(std::vector<T>& values) {
	return values[static_cast<std::size_t>(
		RandInt(0, static_cast<int>(values.size()) - 1))];
}

}

#endif
