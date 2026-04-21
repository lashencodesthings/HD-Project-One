#include "splashkit.h"
#include "../../Entities/Player/player.h"
#include "camera.h"

void update_camera(Camera &camera, const Player player)
{
    double desired_x = player.x - screen_width() / 2;
    double desired_y = player.y - screen_height() / 2;

    camera.x += (desired_x - camera.x) * camera.smooth_factor;
    camera.y += (desired_y - camera.y) * camera.smooth_factor;
}