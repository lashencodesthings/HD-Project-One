#include "splashkit.h"
#include "player.h"
#include "../../Generation/World/World.h"

const double GRAVITY = 0.4f;
const double MOVE_SPEED = 6.0f;
const double JUMP_FORCE = -10.0f;
const double MAX_FALL_SPEED = 12.0f;

const int PLAYER_SIZE = 16;

void update_player(Player &player, World &world)
{
    player.vy += GRAVITY;
    if (player.vy > MAX_FALL_SPEED) player.vy = MAX_FALL_SPEED;

    player.vx = 0;
    if (key_down(A_KEY) || key_down(LEFT_KEY)) player.vx = -MOVE_SPEED;
    if (key_down(D_KEY) || key_down(RIGHT_KEY)) player.vx = MOVE_SPEED;

    if ((key_typed(W_KEY) || key_typed(UP_KEY) || key_typed(SPACE_KEY)) && is_on_ground(player, world))
    {
        player.vy = JUMP_FORCE;
    }

    player.x += player.vx; 
    horizontal_collision(player, world);
    
    player.y += player.vy;
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

// I coded this such that it allows the player to step up 1 block maximum
void horizontal_collision(Player &player, World &world)
{
    int tile_size = world.BLOCK_SIZE * world.zoom;

    if (player.vx > 0)
    {
        // Check if the right side would collide at current height
        if (is_colliding(world, player.x + player.vx, player.y, player.w, player.h))
        {
            // Check if moving up 1 block avoids collision
            if (!is_colliding(world, player.x + player.vx, player.y - tile_size, player.w, player.h))
            {
                player.y -= tile_size; // Step up 1 block
                player.x += player.vx;
            }
            else
            {
                player.vx = 0; // Blocked so player can't walk in that direction
            }
        }
        else
        {
            player.x += player.vx; // No collision so the player move normally
        }
    }
    else if (player.vx < 0) // Check if the left side would collide this time
    {
        if (is_colliding(world, player.x + player.vx, player.y, player.w, player.h))
        {
            if (!is_colliding(world, player.x + player.vx, player.y - tile_size, player.w, player.h))
            {
                player.y -= tile_size; // Step up 1 block
                player.x += player.vx;
            }
            else
            {
                player.vx = 0; // Blocked so the player cannot move in that direction
            }
        }
        else
        {
            player.x += player.vx; //  No collision, move normally
        }
    }
}

void vertical_collision(Player &player, World &world)
{
    if (is_colliding(world, player.x, player.y, player.w, player.h))
    {
        int tile_size = world.BLOCK_SIZE * world.zoom;
        
        if (player.vy > 0)
        {
            int tile_y = (int)(player.y + player.h) / tile_size;
            player.y = (tile_y * tile_size) - player.h;
        }
        else if (player.vy < 0)
        {
            int tile_y = (int)(player.y) / tile_size;
            player.y = (tile_y + 1) * tile_size;
        }
        player.vy = 0;
    }
}

bool is_on_ground(Player &player, World &world)
{
    return is_colliding(world, player.x, player.y + 1, player.w, player.h);
}