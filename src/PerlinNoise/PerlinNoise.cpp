#include "splashkit.h"
#include "PerlinNoise.h"

Noise::Noise(const unsigned int seed) : perlin(seed) {}

double Noise::value(point_2d point)
{
    return perlin.octave2D_01(point.x * zoom, point.y * zoom, octaves);
}