#include "splashkit.h"
#include "World/World.h"
#include "CellularAutomata/CellularAutomata.h"

World *world = nullptr;
CellularAutomata *ca = nullptr;

void load_world()
{
    WorldConfig cfg = load_world_config("world.json");

    if (world) delete world;
    world = new World(960, 540, 12345, 1, cfg);
    world -> generate();

    if (ca) delete ca;
    ca = new CellularAutomata(*world);
}

int main()
{
    open_window("Game", 1920, 1080);

    while (!quit_requested())
    {
        process_events();

        if (key_typed(SPACE_KEY))
        {
            clear_screen();
            load_world();

            if (ca) ca->run();
            if (world) world->draw();
            
            refresh_screen();
        }
    }

    if (ca) delete ca;
    if (world) delete world;

    return 0;
}