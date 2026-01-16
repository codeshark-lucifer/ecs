#pragma once

#include "./entity.hpp"
#include <unordered_map>
#include <memory>

using EntityMap = std::unordered_map<EntityID, std::unique_ptr<Entity>>;

// ================= Entity helpers =================
Entity* FindEntity(EntityID id, const EntityMap& entities);
Entity* CreateEntity(const char* name, EntityMap& entities);

// ================= Transform system =================
void UpdateTransforms(EntityMap& entities);
