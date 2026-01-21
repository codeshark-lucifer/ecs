#include <ecs/world.hpp>
#include <cstdio>
#include <ecs/ecs.hpp>

// World Methods

Entity *World::AddEntity(const char *name)
{
    auto newEntity = std::make_unique<Entity>(++Entity::nextID, name);
    Entity *ptr = newEntity.get();
    entities[ptr->GetID()] = std::move(newEntity);
    ptr->AddComponent<Transform>(); // Always add a transform component
    printf("[World] Entity %s (%u) added\n", name, ptr->GetID());
    return ptr;
}

void World::RemoveEntity(EntityID id)
{
    entities.erase(id);
    printf("[World] Entity %u removed\n", id);
}