#pragma once
#include "../../Entities/Camera/camera.h"
#include "../../Generation/World/World.h"
#include "../../Entities/Entity/entity.h"

struct Villager : Entity {                
    // Inherit from entity
    Villager(double x, double y, double w, double h) : Entity(x, y, w, h) {}
};