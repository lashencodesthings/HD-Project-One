#pragma once
#include "splashkit.h"
#include "../../libraries/perlinnoise.hpp"

class Noise {
    public:
        Noise(const unsigned int seed);
        double value(point_2d pos);
    private:
        siv::PerlinNoise perlin;
        static constexpr int octaves = 4;
        static constexpr float zoom  = 0.05f;
};