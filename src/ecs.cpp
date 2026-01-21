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
    return nullptr; // thread-safe: real pointer later
}

// ----------------- Transform Component -----------------

void Transform::UpdateLocal()
{
    localMatrix =
        mat4::Translate(position) *
        mat4::Rotate(rotation) *
        mat4::Scale(scale);
}

// ----------------- Hierarchy Update (Recursive) -----------------

static void UpdateTransformRecursive(
    World& world,
    EntityID id,
    const mat4& parentWorld)
{
    Entity* e = FindEntity(id, world.GetEntities());
    if (!e) return;

    auto* t = e->GetComponent<Transform>();
    if (!t) return;

    // local
    t->UpdateLocal();

    // world
    t->worldMatrix = parentWorld * t->localMatrix;

    // children
    for (EntityID child : t->children)
    {
        UpdateTransformRecursive(world, child, t->worldMatrix);
    }
}

// ----------------- Transform System -----------------

void UpdateTransforms(World& world, const float& dt)
{
    auto& entities = world.GetEntities();

    // start only from roots
    for (auto& [id, entity] : entities)
    {
        auto* t = entity->GetComponent<Transform>();
        if (!t) continue;

        if (t->parent == 0)
        {
            UpdateTransformRecursive(world, id, mat4::Identity());
        }
    }
}

// ----------------- Parent / Child Linking -----------------

void SetParent(World& world, EntityID child, EntityID parent)
{
    Entity* c = FindEntity(child, world.GetEntities());
    Entity* p = FindEntity(parent, world.GetEntities());
    if (!c || !p) return;

    auto* ct = c->GetComponent<Transform>();
    auto* pt = p->GetComponent<Transform>();
    if (!ct || !pt) return;

    // remove from old parent if needed (optional improvement)

    ct->parent = parent;
    pt->children.push_back(child);
}
