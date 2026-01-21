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

    // Queue commands from main thread
    void QueueAddEntity(const char* name);
    void QueueRemoveEntity(EntityID id);

    // Apply queued commands in update thread
    void FlushCommands();

    // Direct access to entities
    EntityMap& GetEntities() { return entities; }

private:
    EntityMap entities;
    std::queue<Command> commandQueue;
    std::mutex cmdMutex;
};
