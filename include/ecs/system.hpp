#pragma once
#include "./entity.hpp"

class System
{
public:
    const char *name = "System";
    System(const char *name) : name(name) {}
    ~System() { Clean(); }
    virtual void Clean() {}

    virtual void Render(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities) {}
    virtual void Resize(const std::unordered_map<EntityID, std::unique_ptr<Entity>>& entities, int w, int h) {}

    virtual void Start(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities) {}
    virtual void Update(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities, const float &deltaTime) {}
};