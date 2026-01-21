#pragma once
#include "./component.hpp"

#include <vector>
#include <unordered_map>
#include <memory>
#include <atomic>
#include <string>
#include <typeindex>

using EntityID = uint32_t;

class Entity
{
public:
    static EntityID nextID; // for managing unique IDs

    Entity(EntityID id, const char *name);
    Entity(const char *name);
    ~Entity() = default;

    EntityID GetID() const { return id; }
    const char *GetName() const { return name.c_str(); }

    // -------- Add Component --------
    template <typename T, typename... Args>
    T *AddComponent(Args &&...args)
    {
        static_assert(std::is_base_of<Component, T>::value,
                      "T must derive from Component");

        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->entity = this;
        T *ptr = comp.get();

        componentMap[typeid(T)] = ptr;
        components.push_back(std::move(comp));
        return ptr;
    }

    // -------- Get Component --------
    template <typename T>
    T *GetComponent() const
    {
        auto it = componentMap.find(typeid(T));
        if (it == componentMap.end())
            return nullptr;
        return static_cast<T *>(it->second);
    }

    // -------- Has Component --------
    template <typename T>
    bool HasComponent() const
    {
        return componentMap.find(typeid(T)) != componentMap.end();
    }

    // -------- All Components --------
    std::vector<Component *> GetAllComponent() const
    {
        std::vector<Component *> result;
        result.reserve(components.size());
        for (const auto &comp : components)
            result.push_back(comp.get());
        return result;
    }

private:
    EntityID id;
    std::string name;

    std::vector<std::unique_ptr<Component>> components;

    // fast lookup by type
    std::unordered_map<std::type_index, Component *> componentMap;
};
