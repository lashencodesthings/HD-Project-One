#pragma once
#include "../../Entities/Camera/Camera.h"
#include "../../Generation/World/World.h"

struct Player {
    double x, y;
    double vx, vy;
    double w, h;
};

void update_player(Player& player, World& world, Camera& camera);
void draw_player(const Player &player, double cam_x, double cam_y);
void horizontal_collision(Player &player, World &world);
void vertical_collision(Player &player, World &world);
bool is_colliding(World& world, double x, double y, double w, double h);
bool is_on_ground(Player &player, World &world);
void handle_mining(Player &player, World &world, double cam_x, double cam_y);