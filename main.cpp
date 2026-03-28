#include "splashkit.h"
#include "Data/WorldGenerationHeaders.h"

int main()
{
    open_window("Game", 1920, 1080);
    World world(960, 540, 1234456u, 1);

    world.generate();
    world.draw();

    refresh_screen(60);
    delay(500000);
}