#include <ecs/entity.hpp>

// Entity constructors
Entity::Entity(EntityID id, const char *name)
    : id(id), name(name)
{
}

Entity::Entity(const char *name)
    : id(++nextID), name(name)
{
}