#include "splashkit.h"
#include "player.h"
#include <cmath>

const double GRAVITY = 0.4, MOVE_SPEED = 2.0, JUMP_FORCE = -12.0, MAX_FALL_SPEED = 12.0, FRICTION = 0.8;

void handle_mining(Player &player, World &world) {
    if (mouse_down(LEFT_BUTTON))
    {
        double current_mouse_x = mouse_x();
        double current_mouse_y = mouse_y();

        double world_mouse_x = current_mouse_x + player.camera.x;
        double world_mouse_y = current_mouse_y + player.camera.y;

        int block_size = world.BLOCK_SIZE * world.zoom;
        int block_x = (int)(world_mouse_x / block_size);
        int block_y = (int)(world_mouse_y / block_size);

        world.remove_block(block_x, block_y);
        // double dist = sqrt(pow(player.x - world_mouse_x, 2) + pow(player.y - world_mouse_y, 2));
        // if (dist < world.BLOCK_SIZE * 5) { 
        //     world.remove_block(block_x, block_y);
        // }
    }
}

void update_player(Player &player, World &world) {
    player.vy = std::min(player.vy + GRAVITY, MAX_FALL_SPEED);

    if (key_down(D_KEY) || key_down(RIGHT_KEY)) player.vx += MOVE_SPEED;
    if (key_down(A_KEY) || key_down(LEFT_KEY))  player.vx -= MOVE_SPEED;
    if ((key_typed(W_KEY) || key_typed(UP_KEY) || key_typed(SPACE_KEY)) && is_on_ground(world, player)) player.vy = JUMP_FORCE;

    // Lambda function for processing movement, capture by reference so changes can be made to player data
    auto process_move = [&](double &pos, double &vel, bool horizontal) {
        int dir = (vel > 0) ? 1 : -1;
        for (int i = 0; i < std::abs((int) vel); i++) {
            pos += dir;
            if (is_colliding_with_world(world, player)) {
                if (horizontal && is_on_ground(world, player)) {
                    bool stepped = false;
                    for (int j = 0; j < world.BLOCK_SIZE * world.zoom; j++) {
                        player.y --;
                        if (!is_colliding_with_world(world, player)) { stepped = true; break; }
                    }
                    if (stepped) { continue; }
                    else { player.y += (player.y - pos); }
                }
                pos -= dir; 
                vel = 0; 
                break;
            }
        }
    };

    process_move(player.x, player.vx, true);
    process_move(player.y, player.vy, false);

    handle_mining(player, world);
    player.vx *= FRICTION;
}