#pragma once
class Entity;

class Component
{
public:
    virtual ~Component() { Clean(); }
    Entity* entity = nullptr;

    virtual void Clean() {}
    virtual void Start() {}
    virtual void Update(const float& deltaTime) {}
    virtual void DrawUI() {}
};
