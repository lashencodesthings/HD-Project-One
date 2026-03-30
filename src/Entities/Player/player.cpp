#include "splashkit.h"
#include "player.h"
#include <cmath>
#include "../../Generation/World/World.h"

const double GRAVITY = 0.4f;
const double MOVE_SPEED = 1.0f;
const double JUMP_FORCE = -10.0f;
const double MAX_FALL_SPEED = 12.0f;

const int PLAYER_SIZE = 16;

void update_player(Player &player, World &world)
{
    player.vy += GRAVITY;
    if (player.vy > MAX_FALL_SPEED) player.vy = MAX_FALL_SPEED;
    
    if (key_down(A_KEY) || key_down(LEFT_KEY)) player.vx += -MOVE_SPEED;
    if (key_down(D_KEY) || key_down(RIGHT_KEY)) player.vx += MOVE_SPEED;

    if ((key_typed(W_KEY) || key_typed(UP_KEY) || key_typed(SPACE_KEY)) && is_on_ground(player, world))
    {
        player.vy = JUMP_FORCE;
    }

    horizontal_collision(player, world);
    vertical_collision(player, world);

    player.vx *= 0.8f;
}

void draw_player(const Player &player, double cam_x, double cam_y)
{
    double screen_x = player.x - cam_x;
    double screen_y = player.y - cam_y;

    fill_rectangle(COLOR_RED, screen_x, screen_y, PLAYER_SIZE, PLAYER_SIZE);
}

bool is_colliding(World& world, double x, double y, double w, double h)
{
    int tile_size = world.BLOCK_SIZE * world.zoom;

    int left = (int)(x) / tile_size;
    int right = (int)(x + w - 1) / tile_size;
    int top = (int)(y) / tile_size;
    int bottom = (int)(y + h - 1) / tile_size;

    for (int check_x = left; check_x <= right; check_x++) {
        for (int check_y = top; check_y <= bottom; check_y++) {
            if (world.is_solid(check_x, check_y)) {
                return true;
            }
        }
    }

    return false;
}

// I encountered an issue where the player couldn't go up 1 block, so I've made this function step them up 1 block smoothly if possible
void horizontal_collision(Player &player, World &world)
{
    if (player.vx == 0) return;

    int tile_size = world.BLOCK_SIZE * world.zoom; 
    int steps = abs((int)player.vx);
    int dir = (player.vx > 0) ? 1 : -1;
    
    // Check ground once. 
    // IMPORTANT: Terraria allows step-up even if slightly airborne 
    // to handle small bumps, but let's stick to grounded for now.
    bool grounded = is_on_ground(player, world);

    for (int i = 0; i < steps; i++)
    {
        player.x += dir;

        if (is_colliding(world, player.x, player.y, player.w, player.h))
        {
            bool stepped_up = false;
            if (grounded && !is_colliding(world, player.x, player.y - tile_size, player.w, player.h))
            {
                for (int j = 0; j < tile_size; j++) {
                    player.y--;
                    if (!is_colliding(world, player.x, player.y, player.w, player.h)) {
                        stepped_up = true;
                        break; 
                    }
                }
            }

            if (!stepped_up)
            {
                player.x -= dir;
                player.vx = 0; 
                break; 
            }
        }
    }
}

void vertical_collision(Player &player, World &world)
{
    int dir = (player.vy > 0) ? 1 : -1;
    int steps = std::abs((int) player.vy);

    for (int i = 0; i < steps; i++)
    {
        player.y += dir;
        if (is_colliding(world, player.x, player.y, player.w, player.h))
        {
            player.y -= dir;
            player.vy = 0;
            break;
        }
    }
}

bool is_on_ground(Player &player, World &world)
{
    return is_colliding(world, player.x, player.y + 1, player.w, player.h);
}