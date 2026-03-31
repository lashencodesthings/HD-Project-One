#include "splashkit.h"
#include "Player.h"
#include "../../Entities/Camera/Camera.h"
#include "../../Generation/World/World.h"
#include <cmath>

const double GRAVITY = 0.4f;
const double MOVE_SPEED = 2.0f;
const double JUMP_FORCE = -12.0f;
const double MAX_FALL_SPEED = 12.0f;

const int PLAYER_WIDTH = 16;
const int PLAYER_HEIGHT = 32;

void update_player(Player &player, World &world, Camera &camera)
{
    player.vy += GRAVITY;
    if (player.vy > MAX_FALL_SPEED)
    {
        player.vy = MAX_FALL_SPEED;
    }
    
    if (key_down(A_KEY) || key_down(LEFT_KEY))
    {
        player.vx += -MOVE_SPEED;
    }
    if (key_down(D_KEY) || key_down(RIGHT_KEY))
    {
        player.vx += MOVE_SPEED;
    }

    if ((key_typed(W_KEY) || key_typed(UP_KEY) || key_typed(SPACE_KEY)) && is_on_ground(player, world))
    {
        player.vy = JUMP_FORCE;
    }

    horizontal_collision(player, world);
    vertical_collision(player, world);

    handle_mining(player, world, camera.x, camera.y);
    player.vx *= 0.8f;
}

void draw_player(const Player &player, double cam_x, double cam_y)
{
    double screen_x = player.x - cam_x;
    double screen_y = player.y - cam_y;

    fill_rectangle(COLOR_RED, screen_x, screen_y, PLAYER_WIDTH, PLAYER_HEIGHT);
}

bool is_colliding(World& world, double x, double y, double w, double h)
{
    int block_size = world.BLOCK_SIZE * world.zoom;

    int left = (int)(x) / block_size;
    int right = (int)(x + w - 1) / block_size;
    int top = (int)(y) / block_size;
    int bottom = (int)(y + h - 1) / block_size;

    for (int check_x = left; check_x <= right; check_x++) {
        for (int check_y = top; check_y <= bottom; check_y++) {
            if (world.is_solid(check_x, check_y)) {
                return true;
            }
        }
    }

    return false;
}

void horizontal_collision(Player &player, World &world)
{
    if (player.vx == 0) return;

    int block_size = world.BLOCK_SIZE * world.zoom; 
    int steps = abs((int) player.vx);
    int dir = (player.vx > 0) ? 1 : -1;
    bool grounded = is_on_ground(player, world);

    for (int i = 0; i < steps; i++)
    {
        player.x += dir;

        if (is_colliding(world, player.x, player.y, player.w, player.h))
        {
            bool stepped_up = false;
            if (grounded && !is_colliding(world, player.x, player.y - block_size, player.w, player.h))
            {
                for (int j = 0; j < block_size; j++) {
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

void handle_mining(Player &player, World &world, double cam_x, double cam_y)
{
    if (mouse_down(LEFT_BUTTON))
    {
        double current_mouse_x = mouse_x();
        double current_mouse_y = mouse_y();

        double world_mouse_x = current_mouse_x + cam_x;
        double world_mouse_y = current_mouse_y + cam_y;

        int block_size = world.BLOCK_SIZE * world.zoom;
        int block_x = (int)(world_mouse_x / block_size);
        int block_y = (int)(world_mouse_y / block_size);

        double dist = sqrt(pow(player.x - world_mouse_x, 2) + pow(player.y - world_mouse_y, 2));
        if (dist < world.BLOCK_SIZE * 5) { 
            world.remove_block(block_x, block_y);
        }
    }
}
