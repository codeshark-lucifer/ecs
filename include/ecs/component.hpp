#pragma once

class Entity;
class Component
{
public:
    ~Component() { Clean(); }
    Entity *entity = nullptr;
    virtual void Clean() {}
    virtual void Start() {}
    virtual void Update(const float &deltaTime) {}
};