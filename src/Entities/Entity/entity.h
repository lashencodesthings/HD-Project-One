#pragma once
#ifndef ENTITY_H
#define ENTITY_H

struct Camera;

struct Entity {
    double x, y, w, h;
    double vx, vy;

    Entity(double creationX, double creationY, double creationW, double creationH, double creationVx = 0.0, double creationVy = 0.0): x(creationX), y(creationY), w(creationW), h(creationH), vx(creationVx), vy(creationVy) {}
};

bool is_colliding_with_world(World &world, Entity &entity);
bool is_on_ground(World &world, Entity &entity);
void draw_entity(const Entity &entity, const Camera &camera);

#endif