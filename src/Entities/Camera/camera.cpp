#include "splashkit.h"
#include "camera.h"

void update_camera(Camera &camera, double x, double y)
{
    double desired_x = x - screen_width() / 2;
    double desired_y = y - screen_height() / 2;

    camera.x += (desired_x - camera.x) * camera.smooth_factor;
    camera.y += (desired_y - camera.y) * camera.smooth_factor;
}