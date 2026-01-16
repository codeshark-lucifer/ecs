#pragma once
#include "./component.hpp"
#include <utils/vector.h>
#include <utils/quat.h>
#include <utils/matrix.h>

#include <unordered_map>
#include <vector>
#include <atomic>
#include <memory>
#include <typeindex>
#include <typeinfo>

using EntityID = uint32_t;
constexpr EntityID INVALID_ENTITY_ID = 0;

struct Transform
{
    vec3 position{};
    quat rotation{};
    vec3 scale{1.0f};

    EntityID parent = INVALID_ENTITY_ID;
    std::vector<EntityID> children;

    mat4 matrix = mat4::Identity();

    mat4 local() const;
};

class Entity
{
public:
    const char* name = "New Entity";
    Transform transform;

public:
    explicit Entity(const char* name);
    EntityID GetID() const;

    template <typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<Component, T>::value,
                      "T must derive from Component");

        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->entity = this;

        T* ptr = comp.get();
        components[std::type_index(typeid(T))].push_back(std::move(comp));
        return ptr;
    }

    template <typename T>
    std::vector<T*> GetComponents()
    {
        std::vector<T*> result;
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end())
        {
            for (auto& compPtr : it->second)
                result.push_back(static_cast<T*>(compPtr.get()));
        }
        return result;
    }

    template <typename T>
    T* GetComponent()
    {
        auto comps = GetComponents<T>();
        return comps.empty() ? nullptr : comps[0];
    }

    std::vector<Component*> GetAllComponent() const;

private:
    std::unordered_map<std::type_index,
        std::vector<std::unique_ptr<Component>>> components;

    EntityID id;

    static EntityID GenID();
};