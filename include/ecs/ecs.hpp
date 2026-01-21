#pragma once
#include "./entity.hpp"
#include "./world.hpp"

#include <utils/matrix.h>
#include <utils/vector.h>
#include <utils/quat.h>

#include <vector>

// ----------------- Helpers -----------------
Entity* FindEntity(EntityID id, const EntityMap& entities);
Entity* CreateEntity(const char* name, World& world);

// ----------------- Transform Component -----------------
struct Transform : public Component
{
    vec3 position{0, 0, 0};
    quat rotation{};
    vec3 scale{1, 1, 1};

    mat4 localMatrix = mat4::Identity();
    mat4 worldMatrix = mat4::Identity();

    EntityID parent = 0;
    std::vector<EntityID> children;

    void UpdateLocal();
};

// ----------------- Transform System -----------------
void UpdateTransforms(World& world, const float& dt);

// ----------------- Hierarchy Helper -----------------
void SetParent(World& world, EntityID child, EntityID parent);
