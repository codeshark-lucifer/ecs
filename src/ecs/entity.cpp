
#include <ecs/entity.hpp>

mat4 Transform::local() const
{
    return mat4::Translate(position)
         * mat4::Rotate(rotation)
         * mat4::Scale(scale);
}

Entity::Entity(const char* name)
    : name(name), id(GenID())
{
}

EntityID Entity::GetID() const
{
    return id;
}

std::vector<Component*> Entity::GetAllComponent() const
{
    std::vector<Component*> result;
    for (const auto& [idx, list] : components)
    {
        for (const auto& comp : list)
        {
            result.push_back(comp.get());
        }
    }
    return result;
}

EntityID Entity::GenID()
{
    static std::atomic<EntityID> counter{1};
    return counter++;
}