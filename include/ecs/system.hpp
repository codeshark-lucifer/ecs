#pragma once
class World;

class System
{
public:
    const char* name;
    explicit System(const char* n) : name(n) {}
    virtual ~System() = default;

    virtual void OnStart(World& world) {}
    virtual void OnUpdate(World& world, float dt) {}
    virtual void OnRender(World& world) {}
    virtual void OnUI(World& world) {}
    virtual void OnResize(World& world, int w, int h) {}
};
