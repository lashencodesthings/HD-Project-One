#include "splashkit.h"
#include "player.h"
#include "../../Generation/World/World.h"

const double GRAVIY = 2.0f;
const double MOVE_SPEED = 20.0f;
const double JUMP_FORCE = -10.0f;
const double MAX_FALL_SPEED = 12.0f;

const int PLAYER_SIZE = 16;

void update_player(Player &player, World &world)
{
    player.vx = 0;

    if (key_down(A_KEY) || key_down(LEFT_KEY))
    {
        player.vx = -MOVE_SPEED;
    }
    if (key_down(D_KEY) || key_down(RIGHT_KEY))
    {
        player.vx = MOVE_SPEED;
    }    
    if (key_down(S_KEY) || key_down(DOWN_KEY))
    {
        player.vy = MOVE_SPEED;
    }
    if (key_down(W_KEY) || key_down(UP_KEY))
    {
        player.vy = -MOVE_SPEED;
    }

    if (key_down(W_KEY) && is_on_ground(player, world))
    {
        player.vy = JUMP_FORCE;
    }

    player.vy += GRAVIY;

    if (player.vy > MAX_FALL_SPEED)
    {
        player.vy = MAX_FALL_SPEED;
    }

    player.x += player.vx;
    player.y += player.vy;

    vertical_collision(player, world);
    horizontal_collision(player, world);

    player.vx *= 0.8f;
    player.vy *= 0.8f;
}

void draw_player(const Player &player, double cam_x, double cam_y)
{
    double screen_x = player.x - cam_x;
    double screen_y = player.y - cam_y;

    fill_rectangle(COLOR_RED, screen_x, screen_y, PLAYER_SIZE, PLAYER_SIZE);
}

// Calculations
void horizontal_collision(Player &player, World &world)
{
    if (is_colliding(world, player.x, player.y, player.w, player.h))
    {
        int tile_size = world.BLOCK_SIZE * world.zoom;

        if (player.vx > 0) {
            int right = (int)(player.x + player.w) / tile_size;
            player.x = right * tile_size - player.w;
        }
        else if (player.vx < 0) {
            int left = (int)(player.x) / tile_size;
            player.x = (left + 1) * tile_size;
        }

        player.vx = 0;
    }
}

void vertical_collision(Player &player, World &world)
{
    if (is_colliding(world, player.x, player.y, player.w, player.h))
    {
        int tile_size = world.BLOCK_SIZE * world.zoom;

        if (player.vy > 0) {
            int bottom = (int)(player.y + player.h) / tile_size;
            player.y = bottom * tile_size - player.h;
        }
        else if (player.vy < 0) {
            int top = (int)(player.y) / tile_size;
            player.y = (top + 1) * tile_size;
        }

        player.vy = 0;
    }
}

bool is_colliding(World& world, float x, float y, float w, float h)
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

bool is_on_ground(Player &player, World &world)
{
    int tile_size = world.BLOCK_SIZE * world.zoom;

    int left  = (int)(player.x) / tile_size;
    int right = (int)(player.x + player.w - 1) / tile_size;

    int y = (int)(player.y + player.h + 1) / tile_size;

    for (int x = left; x <= right; x++) {
        if (world.is_solid(x, y)) {
            return true;
        }
    }

    return false;
}