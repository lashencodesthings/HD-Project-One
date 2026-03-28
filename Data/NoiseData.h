#include "./PerlinNoise.hpp"
#include "WorldGenerationData.h"
#include "splashkit.h"

const struct NOISE_DATA
{
    const siv::PerlinNoise perlin;
    const int octaves;
    const float zoom;

    NOISE_DATA(WORLD_GENERATION_DATA world, int octave_value, float zoom_value) : perlin{ world.seed }, octaves(octave_value), zoom(zoom_value) {}

    const double value(const point_2d &point)
    {
        return perlin.octave2D_01((point.x * zoom), (point.y * zoom), octaves);
    }
};