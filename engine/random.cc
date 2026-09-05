#include "engine/random.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>

namespace engine {

namespace {

bool deterministic = false;

}  // namespace

std::mt19937& Random::RNG() {  // NOLINT(readability-identifier-naming)
	static auto* generator = new std::mt19937(std::random_device()());
	return *generator;
}

int Random::IntRandomRange(int a, int b) {
	std::uniform_int_distribution<int> distribution(a, b);
	return distribution(RNG());
}

void Random::Randomize() {
	if (deterministic) {
		return;
	}
	RNG().seed(static_cast<unsigned int>(
		std::chrono::steady_clock::now().time_since_epoch().count()));
}

void Random::SetDeterministicSeed(unsigned int seed) {
	deterministic = true;
	RNG().seed(seed);
}

int Random::IntRandom(int n) {
	if (n <= 0) {
		return 0;
	}
	return IntRandomRange(0, n);
}

double Random::DoubleRandomRange(double a, double b) {
	// the distribution stops just before its upper bound, so go one step past b
	std::uniform_real_distribution<double> distribution(
		a, std::nextafter(b, std::numeric_limits<double>::max()));
	return distribution(RNG());
}

float Random::FloatRandomRange(float a, float b) {
	// same as for doubles, go one step past b so b can come out
	std::uniform_real_distribution<float> distribution(
		a, std::nextafter(b, std::numeric_limits<float>::max()));
	// float rounding can still land on that step past b, so cut it off
	return std::min(distribution(RNG()), b);
}

float Random::FloatRandom(float n) {
	if (n <= 0.0f) {
		return 0.0f;
	}
	return FloatRandomRange(0.0f, n);
}

double Random::DoubleRandom(double n) {
	if (n <= 0.0) {
		return 0.0;
	}
	return DoubleRandomRange(0.0, n);
}

}  // namespace engine
