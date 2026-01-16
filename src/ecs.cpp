#include <ecs/ecs.hpp>

Entity* FindEntity(EntityID id, const EntityMap& entities)
{
    auto it = entities.find(id);
    return (it != entities.end()) ? it->second.get() : nullptr;
}

Entity* CreateEntity(const char* name, EntityMap& entities)
{
    auto entity = std::make_unique<Entity>(name);
    EntityID id = entity->GetID();

    auto [it, inserted] = entities.emplace(id, std::move(entity));
    return it->second.get();
}

static mat4 ComputeWorldMatrix(Entity* entity, const EntityMap& entities)
{
    mat4 result = mat4::Identity();

    while (entity)
    {
        result = entity->transform.local() * result;

        EntityID parentID = entity->transform.parent;
        if (parentID == INVALID_ENTITY_ID)
            break;

        entity = FindEntity(parentID, entities);
    }

    return result;
}

void UpdateTransforms(EntityMap& entities)
{
    for (auto& [id, entity] : entities)
    {
        entity->transform.matrix = ComputeWorldMatrix(entity.get(), entities);
    }
}
