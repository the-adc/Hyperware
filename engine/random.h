#ifndef ENGINE_RANDOM_H
#define ENGINE_RANDOM_H

#include <algorithm>
#include <random>
#include <vector>

std::mt19937 &rng();

int randint(int a, int b);

template <typename T>
void shuffle(std::vector<T> &values) {
    std::shuffle(values.begin(), values.end(), rng());
}

template <typename T>
T &choice(std::vector<T> &values) {
    return values[(std::size_t)randint(0, (int)values.size() - 1)];
}

#endif
