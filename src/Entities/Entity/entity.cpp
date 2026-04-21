#include "splashkit.h"
#include "../../Generation/World/World.h"
#include "../../Entities/Entity/entity.h"
#include <cmath>

void draw_entity(const Entity &entity, const Camera &camera) { fill_rectangle(COLOR_RED, entity.x - camera.x, entity.y - camera.y, entity.w, entity.h); }

struct Hitbox { int left, right, top, bottom; };

bool is_colliding_with_world(World &world, Entity &entity)
{
    int block_size = world.BLOCK_SIZE * world.zoom;

    int left   = (int)(entity.x) / block_size;
    int right  = (int)(entity.x + entity.w - 1) / block_size;
    int top    = (int)(entity.y) / block_size;
    int bottom = (int)(entity.y + entity.h - 1) / block_size;

    for (int check_x = left; check_x <= right; check_x++) {
        for (int check_y = top; check_y <= bottom; check_y++) { 
            if (world.is_solid(check_x, check_y)) { return true; }
        }
    }
    return false;
}

bool is_on_ground(World &world, Entity &entity) {
    entity.y += 1;
    bool colliding = is_colliding_with_world(world, entity);
    entity.y -= 1;
    return colliding;
}