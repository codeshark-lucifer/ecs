#include <ecs/world.hpp>

EntityID World::CreateEntity()
{
    EntityID id;

    if (!freeList.empty())
    {
        id = freeList.back();
        freeList.pop_back();
        alive[id] = true;
    }
    else
    {
        id = nextID++;
        alive.push_back(true);
    }

    return id;
}

void World::DestroyEntity(EntityID id)
{
    if (id < alive.size() && alive[id])
    {
        alive[id] = false;
        freeList.push_back(id);
        // also remove all components of this entity
    }
}
