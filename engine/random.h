#ifndef HYPER_ENGINE_RANDOM_H_
#define HYPER_ENGINE_RANDOM_H_

#include <random>

namespace engine {

class Random {
public:
	Random() = delete;

	static std::mt19937& RNG();  // NOLINT(readability-identifier-naming)

	static int IntRandomRange(int a, int b);

	// Real number from a to b, both included. a can't be greater than b.
	static double DoubleRandomRange(double a, double b);

	// Same as DoubleRandomRange but for floats.
	static float FloatRandomRange(float a, float b);

	// Seeds generator from the clock.
	static void Randomize();

	static void SetDeterministicSeed(unsigned int seed);

	static int IntRandom(int n);

	static double DoubleRandom(double n);

	static float FloatRandom(float n);
};

}  // namespace engine

#endif  // HYPER_ENGINE_RANDOM_H_
