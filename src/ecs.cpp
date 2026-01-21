#include <ecs/ecs.hpp>

// Static member definition
EntityID Entity::nextID = 0;

// Entity constructors
Entity::Entity(EntityID id, const char *name)
    : id(id), name(name)
{
}

Entity::Entity(const char *name)
    : id(++nextID), name(name)
{
}

// ----------------- Helpers -----------------

Entity *FindEntity(EntityID id, const EntityMap &entities)
{
    auto it = entities.find(id);
    if (it != entities.end())
        return it->second.get();
    return nullptr;
}

Entity *CreateEntity(const char *name, World &world)
{
    return world.AddEntity(name);
}

// ----------------- World Methods -----------------

Entity *World::AddEntity(const char *name)
{
    EntityID newID = ++Entity::nextID;
    auto newEntity = std::make_unique<Entity>(newID, name);
    Entity *ptr = newEntity.get();
    entities[newID] = std::move(newEntity);
    ptr->AddComponent<Transform>(); // Always add a transform component
    return ptr;
}

void World::RemoveEntity(EntityID id)
{
    entities.erase(id);
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
    World &world,
    EntityID id,
    const mat4 &parentWorld)
{
    Entity *e = FindEntity(id, world.GetEntities());
    if (!e)
        return;

    auto *t = e->GetComponent<Transform>();
    if (!t)
        return; // Should not happen as Transform is always added

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

void UpdateTransforms(World &world, const float &dt)
{
    auto &entities = world.GetEntities();

    // start only from roots
    for (auto &[id, entity] : entities)
    {
        auto *t = entity->GetComponent<Transform>();
        if (!t)
            continue; // Should not happen as Transform is always added

        if (t->parent == 0)
        {
            UpdateTransformRecursive(world, id, mat4::Identity());
        }
    }
}

// ----------------- Parent / Child Linking -----------------

void SetParent(World &world, EntityID child, EntityID parent)
{
    Entity *c = FindEntity(child, world.GetEntities());
    Entity *p = FindEntity(parent, world.GetEntities());
    if (!c || !p)
        return;

    auto *ct = c->GetComponent<Transform>();
    auto *pt = p->GetComponent<Transform>();
    if (!ct || !pt)
        return; // Should not happen as Transform is always added

    // remove from old parent if needed (optional improvement)

    ct->parent = parent;
    pt->children.push_back(child);
}
