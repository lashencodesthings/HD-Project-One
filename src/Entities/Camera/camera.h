#pragma once


struct Camera
{
    float x, y;  // Camera position
    float smooth_factor = 0.1f; // How fast the camera follows the player
};

void update_camera(Camera &cam, float target_x, float target_y);