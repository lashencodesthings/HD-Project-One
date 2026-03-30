#include "camera.h"
#include "splashkit.h"

void update_camera(Camera &cam, float target_x, float target_y)
{
    cam.x = target_x - screen_width() / 2;
    cam.y = target_y - screen_height() / 2;
}