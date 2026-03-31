#pragma once
#include <random>
#include <vector>

class World; 

class CellularAutomata {
    public:
        CellularAutomata(World& world_data);

        void run();

    private:
        void seed_noise();
        void apply();
        bool add_air_hole();
        void smooth();

        World& world;
        std::mt19937 rng;

        const int WALL_THRESHOLD = 4;
        const int NOISE_DENSITY = 38;
        const int ITERATIONS = 5;

        std::uniform_int_distribution<int> percent_dist{0, 99};
        int count_neighbours(int x, int y);
        void clean_dust();
};