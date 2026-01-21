#pragma once
#include "./entity.hpp"
#include <unordered_map>
#include <memory>
#include <queue>
#include <mutex>

using EntityMap = std::unordered_map<EntityID, std::unique_ptr<Entity>>;

enum class CommandType { AddEntity, RemoveEntity };

struct Command
{
    CommandType type;
    std::string name; // used for AddEntity
    EntityID id;      // used for RemoveEntity
};

class World
{
public:
    World() = default;

    // Push commands from main thread
    void QueueAddEntity(const char* name)
    {
        std::scoped_lock lock(cmdMutex);
        Command cmd;
        cmd.type = CommandType::AddEntity;
        cmd.name = name;
        commandQueue.push(cmd);
    }

    void QueueRemoveEntity(EntityID id)
    {
        std::scoped_lock lock(cmdMutex);
        Command cmd;
        cmd.type = CommandType::RemoveEntity;
        cmd.id = id;
        commandQueue.push(cmd);
    }

    // Apply queued commands (thread-safe)
    void FlushCommands()
    {
        std::scoped_lock lock(cmdMutex);
        while (!commandQueue.empty())
        {
            Command cmd = commandQueue.front();
            commandQueue.pop();

            if (cmd.type == CommandType::AddEntity)
            {
                auto e = std::make_unique<Entity>(cmd.name.c_str());
                EntityID id = e->GetID();
                entities.emplace(id, std::move(e));
                printf("[World] Entity %s (%u) added\n", cmd.name.c_str(), id);
            }
            else if (cmd.type == CommandType::RemoveEntity)
            {
                entities.erase(cmd.id);
                printf("[World] Entity %u removed\n", cmd.id);
            }
        }
    }

    EntityMap& GetEntities() { return entities; }

private:
    EntityMap entities;
    std::queue<Command> commandQueue;
    std::mutex cmdMutex;
};