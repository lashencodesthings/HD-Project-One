#pragma once
#include "../../Generation/World/World.h"

struct Player {
    float x, y;
    float w, h;
    float vx, vy;
};

void update_player(Player& player, World& world);
void draw_player(const Player &player, double cam_x, double cam_y);
void horizontal_collision(Player &player, World &world);
void vertical_collision(Player &player, World &world);
bool is_colliding(World& world, float x, float y, float w, float h);