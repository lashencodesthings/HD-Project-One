#include "splashkit.h"
#include "World/World.h"
#include "CellularAutomata/CellularAutomata.h"

int main()
{
    open_window("Game", 1920, 1080);
    World world(960, 540, 52u, 1);

    world.generate();

    CellularAutomata ca(world);
    ca.run();

    world.draw();

    refresh_screen(60);
    delay(500000);
}