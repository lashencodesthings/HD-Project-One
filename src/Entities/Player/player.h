#pragma once
#include "../../Entities/Camera/Camera.h"
#include "../../Generation/World/World.h"
#include "../../Entities/Entity/entity.h"

struct Player : Entity 
{ 
    using Entity::Entity; // Inherited values from entity 
    Camera camera;

    Player(double creationX, double creationY, double creationW, double creationH, double creationVx, double creationVy, Camera creationCamera) : Entity(creationX, creationY, creationW, creationH, creationVx, creationVy), camera(creationCamera){}
};

void update_player(Player& player, World& world);
void handle_mining(Player &player, World &world);