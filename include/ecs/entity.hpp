#pragma once
#include "./component.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <atomic>
#include <string>

using EntityID = uint32_t;

class Entity
{
public:
    Entity(const char* name);
    ~Entity() = default;

    EntityID GetID() const;
    const char* GetName() const { return name.c_str(); }

    void AddComponent(std::unique_ptr<Component> c)
    {
        c->entity = this;
        components.push_back(std::move(c));
    }

    std::vector<Component*> GetAllComponent() const
    {
        std::vector<Component*> result;
        for (const auto& comp : components)
            result.push_back(comp.get());
        return result;
    }

private:
    static EntityID GenID();

    EntityID id;
    std::string name;
    std::vector<std::unique_ptr<Component>> components;
};
