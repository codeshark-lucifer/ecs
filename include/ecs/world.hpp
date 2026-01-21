#pragma once
#include "./entity.hpp"
#include <unordered_map>
#include <memory>
#include <queue>
#include <mutex>
#include <string>

using EntityMap = std::unordered_map<EntityID, std::unique_ptr<Entity>>;

enum class CommandType { AddEntity, RemoveEntity };

struct Command
{
    CommandType type;
    std::string name; // for AddEntity
    EntityID id;      // for RemoveEntity
};

class World
{
public:
    World() = default;

    Entity *AddEntity(const char *name);
    void RemoveEntity(EntityID id);

    // Direct access to entities
    EntityMap &GetEntities() { return entities; }

private:
    EntityMap entities;
    // std::queue<Command> commandQueue;
    // std::mutex cmdMutex;
};
