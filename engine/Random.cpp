#include "engine/Random.h"

std::mt19937 &rng() {
    static std::random_device device;
    static std::mt19937 generator(device());
    return generator;
}

int randint(int a, int b) {
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(rng());
}
