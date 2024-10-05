#pragma once

#include "BasicComponents.h"
#include "entt/entity/fwd.hpp"

enum COLLISION_BITS{
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};
inline void checkAABBCollision(const Cast::Transform& a, const Cast::Transform& b, Cast::Collidable& aCollidable, Cast::Collidable& bCollidable) {

    bool collision = (a.position.x < b.position.x + b.dimensions.x &&
            a.position.x + a.dimensions.x > b.position.x &&
            a.position.y < b.position.y + b.dimensions.y &&
            a.position.y + a.dimensions.y > b.position.y);

    if(!collision){
        aCollidable.isColliding = collision;
        bCollidable.isColliding = collision;
        aCollidable.bitmask = 0;
        bCollidable.bitmask = 0;
        return;
    }

    uint8_t packedA{0}, packedB{0};

    // Determine collision sides for 'a'
    packedA |= (a.position.x < b.position.x) << COLLISION_BITS::LEFT;
    packedA |= (a.position.x + a.dimensions.x > b.position.x + b.dimensions.x) << COLLISION_BITS::RIGHT;
    packedA |= (a.position.y < b.position.y) << COLLISION_BITS::TOP;
    packedA |= (a.position.y + a.dimensions.y > b.position.y + b.dimensions.y) << COLLISION_BITS::BOTTOM;

    // Inverse logic for 'b'
    packedB |= (b.position.x < a.position.x) << COLLISION_BITS::LEFT;
    packedB |= (b.position.x + b.dimensions.x > a.position.x + a.dimensions.x) << COLLISION_BITS::RIGHT;
    packedB |= (b.position.y < a.position.y) << COLLISION_BITS::TOP;
    packedB |= (b.position.y + b.dimensions.y > a.position.y + a.dimensions.y) << COLLISION_BITS::BOTTOM;

    aCollidable.bitmask = packedA;
    bCollidable.bitmask = packedB;

    // Set collision flags based on the bitmask values
    aCollidable.isColliding = packedA != 0;
    bCollidable.isColliding = packedB != 0;
}

inline void collisionSystem(entt::registry& registry){
    using namespace Cast;
    auto view = registry.view<Cast::Transform, Cast::Collidable>();

    for (auto entity : view) {
        view.get<Collidable>(entity).isColliding = false;
    }

    for (auto entityA : view) {
        const auto& transformA = view.get<Transform>(entityA);

        for (auto entityB : view) {
            if (entityA == entityB) continue;  // Don't check the entity with itself

            const auto& transformB = view.get<Transform>(entityB);

            checkAABBCollision(transformA, transformB, view.get<Collidable>(entityA), view.get<Collidable>(entityB));
        }
    }
}
