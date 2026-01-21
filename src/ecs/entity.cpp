#include <ecs/entity.hpp>

static std::atomic<EntityID> g_NextID{1};

Entity::Entity(const char* name)
    : id(GenID()), name(name)
{
}

EntityID Entity::GenID()
{
    return g_NextID.fetch_add(1, std::memory_order_relaxed);
}
