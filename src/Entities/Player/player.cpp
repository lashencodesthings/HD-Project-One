#include "splashkit.h"
#include "Player.h"
#include <cmath>

const double GRAVITY = 0.4, MOVE_SPEED = 2.0, JUMP_FORCE = -12.0, MAX_FALL_SPEED = 12.0, FRICTION = 0.8;

void handle_mining(Player &player, World &world) {
    if (!mouse_down(LEFT_BUTTON)) { return; }

    point_2d mouse_world = { mouse_x() + player.camera.x, mouse_y() + player.camera.y };
    point_2d player_center = { player.x + (player.w / 2), player.y + (player.h / 2) };

    int scaled_size = world.BLOCK_SIZE * world.zoom;
    world.remove_block((int) (mouse_world.x / scaled_size), (int) (mouse_world.y / scaled_size));
    
    // if (point_point_distance(player_center, mouse_world) < scaled_size * 5) {
    // }
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
                    if (stepped) continue; else player.y += (player.y - pos);
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