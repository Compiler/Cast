#pragma once

#include "BasicComponents.h"
#include "entt/entity/fwd.hpp"


inline bool checkAABBCollision(const Cast::Transform& a, const Cast::Transform& b) {
    return (a.position.x < b.position.x + b.dimensions.x &&
            a.position.x + a.dimensions.x > b.position.x &&
            a.position.y < b.position.y + b.dimensions.y &&
            a.position.y + a.dimensions.y > b.position.y);
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

            if (checkAABBCollision(transformA, transformB)) {
                view.get<Collidable>(entityA).isColliding = true;
                view.get<Collidable>(entityB).isColliding = true;
            }
        }
    }
}
