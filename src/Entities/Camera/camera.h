#pragma once
struct Camera
{
    double x, y;
    double smooth_factor = 0.1f;
};

void update_camera(Camera &camera, Player &player);