#include <ecs/ecs.hpp>

// ----------------- Helpers -----------------
Entity* FindEntity(EntityID id, const EntityMap& entities)
{
    auto it = entities.find(id);
    if (it != entities.end()) return it->second.get();
    return nullptr;
}

Entity* CreateEntity(const char* name, World& world)
{
    world.QueueAddEntity(name);
    return nullptr; // cannot return actual pointer immediately (thread-safe)
}

// ----------------- Transform Component -----------------
void Transform::Update(const float& dt)
{
    // compute local transform
    matrix = mat4::Translate(position) * mat4::Rotate(rotation) * mat4::Scale(scale);
}

// ----------------- Transform System -----------------
void UpdateTransforms(World& world, const float& dt)
{
    for (auto& [id, entity] : world.GetEntities())
    {
        for (auto* comp : entity->GetAllComponent())
        {
            if (auto* t = dynamic_cast<Transform*>(comp))
            {
                t->Update(dt);
            }
        }
    }
}
