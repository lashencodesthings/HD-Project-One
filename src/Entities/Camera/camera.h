#pragma once

struct Camera
{
    float x, y;
};

void update_camera(Camera &cam, float target_x, float target_y);