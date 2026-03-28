#include "splashkit.h"
#include "WorldGenerationHeaders.h"

int main()
{
    WORLD_GENERATION_DATA world = { 4200, 1200, 123456u };
    NOISE_DATA noise = NOISE_DATA(world, 4, 0.01);

    for(int x = 0; x < world.width; x++)
    {
        for(int y = 0; y < world.height; y++)
        {
			double val = noise.value({1, 23});			
        }
    }
}