#include "splashkit.h"
#include "Generation/World/World.h"
#include "Generation/CellularAutomata/CellularAutomata.h"
#include "Entities/Player/player.h"
#include "Entities/Camera/camera.h"

World generate_world()
{
    WorldConfig cfg = load_world_config("world.json");

    World world(960, 540, 12345, 1, cfg);
    world.generate();

    CellularAutomata ca(world);
    ca.run();

    return world;
}

int main()
{
    Player player = {100, 100, 0, 0};
    Camera cam = {0, 0};

    open_window("Game", 1920, 1080);

    World world = generate_world();

    while (!quit_requested())
    {
        process_events();

        update_player(player);
        update_camera(cam, player.x, player.y);

        clear_screen(COLOR_WHITE);

        world.draw(cam.x, cam.y);
        draw_player(player, cam.x, cam.y);

        refresh_screen(60);
    }

    return 0;
}