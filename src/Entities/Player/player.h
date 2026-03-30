#pragma once

struct Player
{
    float x, y;
    float vx, vy;
};

void update_player(Player &player);
void draw_player(const Player &player, float cam_x, float cam_y);