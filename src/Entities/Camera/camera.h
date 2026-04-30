#pragma once
#ifndef CAMERA_H
#define CAMERA_H

struct Camera
{
    double x, y;
    double smooth_factor = 0.1f;
};

void update_camera(Camera &camera, double x, double y);

#endif