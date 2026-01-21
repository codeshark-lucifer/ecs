#include <ecs/entity.hpp>

Entity::Entity(const char* n)
    : name(n), id(GenID()) {}

EntityID Entity::GetID() const { return id; }

EntityID Entity::GenID()
{
    static std::atomic<EntityID> counter{1};
    return counter++;
}
