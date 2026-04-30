#include "splashkit.h"
#include "Generation/World/World.h"
#include "Generation/CellularAutomata/CellularAutomata.h"
#include "Entities/Player/player.h"
#include "Entities/Camera/camera.h"
#include "Entities/Entity/entity.h"
#include <memory>

const int WORLD_WIDTH = 6400;
const int WORLD_HEIGHT = 1800;

std::unique_ptr<World> generate_world()
{
    WorldConfig cfg = load_world_config("world.json");

    std::unique_ptr<World> new_world = std::make_unique<World>(WORLD_WIDTH, WORLD_HEIGHT, 9878685, 1, cfg);
    new_world->generate();

    CellularAutomata ca(*new_world);
    ca.run();

    return new_world;
}

int main()
{
    open_window("Game", 1920, 1080);

    std::unique_ptr<World> world = generate_world();

    point_2d spawn = world->get_random_spawn_point();

    Camera camera;
    Player player = {spawn.x, spawn.y - 5, 16, 32, 0, 0, camera};

    while (!quit_requested())
    {
        process_events();

        update_player(player, *world);
        update_camera(camera, player.x, player.y);

        clear_screen(COLOR_LIGHT_SKY_BLUE);

        world->draw(camera.x, camera.y);
        draw_entity(player, camera);

        refresh_screen(60);
        
        BiomeType biome = get_biome_at_x(player.x); 
        switch(biome)
        {
            case BiomeType::Plains:
                write_line("Plains");
                break;
            case BiomeType::Desert:
                write_line("Desert");
                break;
            case BiomeType::Snow:
                write_line("Snow");
                break;
            case BiomeType::Jungle:
                write_line("Jungle");
                break;
            default:
                write_line("Error, no biome found");
                break;
        }
        if (key_typed(R_KEY))
        {
            world = generate_world();
            point_2d spawn = world->get_random_spawn_point();
            player.x = spawn.x;
            player.y = spawn.y - 1;
        }
    }

    return 0;
}

// Run clang++ -std=c++20 (Get-ChildItem -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }) -I src -l splashkit -o test -Wall