#include "splashkit.h"
#include "Generation/World/World.h"
#include "Generation/CellularAutomata/CellularAutomata.h"
#include "Entities/Player/player.h"
#include "Entities/Camera/camera.h"

const int WORLD_WIDTH = 6400;
const int WORLD_HEIGHT = 1800;

World generate_world()
{
    WorldConfig cfg = load_world_config("world.json");

    World world(WORLD_WIDTH, WORLD_HEIGHT, 9878685, 1, cfg);
    world.generate();

    CellularAutomata ca(world);
    ca.run();

    return world;
}

int main()
{
    open_window("Game", 1920, 1080);

    World world = generate_world();

    point_2d spawn = world.get_random_spawn_point();

    Player player = {
        spawn.x,
        spawn.y - 10,
        0, 0,
        16, 16
    };

    Camera cam = {player.x, player.y};

    while (!quit_requested())
    {
        process_events();

        update_player(player, world);
        update_camera(cam, player.x, player.y);

        clear_screen(COLOR_LIGHT_SKY_BLUE);

        world.draw(cam.x, cam.y);
        draw_player(player, cam.x, cam.y);

        refresh_screen(60);
    }

    return 0;
}