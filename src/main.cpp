#include "splashkit.h"
#include "Generation/World/World.h"
#include "Generation/CellularAutomata/CellularAutomata.h"
#include "Entities/Player/Player.h"
#include "Entities/Camera/Camera.h"
#include "Entities/Entity/Entity.h"
#include <memory>

// const int WORLD_WIDTH = 6400;
// const int WORLD_HEIGHT = 1800;
const int WORLD_WIDTH = 50;
const int WORLD_HEIGHT = 1800;

std::unique_ptr<World> generate_world()
{
    WorldConfig cfg = load_world_config("world.json");

    std::unique_ptr<World> new_world = std::make_unique<World>(WORLD_WIDTH, WORLD_HEIGHT, 9878685, 1, cfg);
    new_world -> generate();

    CellularAutomata ca(*new_world);
    ca.run();

    return new_world;
}

int main()
{
    open_window("Game", 1920, 1080);

    std::unique_ptr<World> world = generate_world();

    point_2d spawn = world -> get_random_spawn_point();
    
    Camera cam;
    Player player = { spawn.x, spawn.y - 1, 16, 32, 0.0, 0.0, cam };

    while (!quit_requested())
    {
        process_events();

        update_player(player, *world);
        update_camera(cam, player);

        clear_screen(COLOR_LIGHT_SKY_BLUE);

        world -> draw(cam.x, cam.y);
        draw_entity(player, cam);

        refresh_screen(60);

        if(key_typed(R_KEY))
        {
            world = generate_world();
            point_2d spawn = world -> get_random_spawn_point();
            player.x = spawn.x;
            player.y = spawn.y - 1;
        }
    }

    return 0;
}

// Run clang++ -std=c++20 (Get-ChildItem -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }) -I src -l splashkit -o test -Wall