#pragma once
// Forward declaration to tell the compiler the Player is a struct that exists

struct Player; 
struct Camera;

void update_camera(Camera &camera, Player &player);