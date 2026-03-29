#pragma once
#include "PerlinNoise.hpp"
#include "splashkit.h"

const int octaves = 4;
const float zoom = 0.05;

class Noise
{
    private:
        siv::PerlinNoise perlin;

    public:        
        Noise(const unsigned int &seed) : perlin { seed } {}

        const double value(const point_2d &point)
        {
            return perlin.octave2D_01((point.x * zoom), (point.y * zoom), octaves);
        }
};