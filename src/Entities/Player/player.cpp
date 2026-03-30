#include "splashkit.h"
#include "player.h"

const float GRAVITY = 0.5f;
const float MOVE_SPEED = 20.0f;
const float JUMP_FORCE = -10.0f;
const float MAX_FALL_SPEED = 12.0f;

const int PLAYER_SIZE = 16;

void update_player(Player &player)
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

    if (key_typed(SPACE_KEY))
    {
        player.vy = JUMP_FORCE;
    }

    // player.vy += GRAVITY;

    if (player.vy > MAX_FALL_SPEED)
    {
        player.vy = MAX_FALL_SPEED;
    }

    player.x += player.vx;
    player.y += player.vy;

    player.vx *= 0.8f;
    player.vy *= 0.8f;
}

void draw_player(const Player &player, float cam_x, float cam_y)
{
    float screen_x = player.x - cam_x;
    float screen_y = player.y - cam_y;

    fill_rectangle(COLOR_RED, screen_x, screen_y, PLAYER_SIZE, PLAYER_SIZE);
}