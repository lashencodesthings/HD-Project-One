#include "camera.h"
#include "splashkit.h"

void update_camera(Camera &cam, float target_x, float target_y)
{
    float desired_x = target_x - screen_width() / 2;
    float desired_y = target_y - screen_height() / 2;

    cam.x += (desired_x - cam.x) * cam.smooth_factor;
    cam.y += (desired_y - cam.y) * cam.smooth_factor;
}