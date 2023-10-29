#include "playerentity.h"

PlayerEntity::PlayerEntity(std::unique_ptr<EntityPhysicsInfo> physicsInfo,
                           ControllerType controllerType)
    : Entity(std::move(physicsInfo)), controllerType_(controllerType) {}
