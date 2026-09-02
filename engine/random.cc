#include "engine/random.h"

namespace engine {

std::mt19937& Rng() {
	static auto* generator = new std::mt19937(std::random_device()());
	return *generator;
}

int RandInt(int a, int b) {
	std::uniform_int_distribution<int> distribution(a, b);
	return distribution(Rng());
}

}
