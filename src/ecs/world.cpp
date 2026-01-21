#include <ecs/world.hpp>
#include <cstdio>
#include <ecs/ecs.hpp>

void World::QueueAddEntity(const char *name)
{
    std::scoped_lock lock(cmdMutex);
    Command cmd;
    cmd.type = CommandType::AddEntity;
    cmd.name = name;
    commandQueue.push(cmd);
}

void World::QueueRemoveEntity(EntityID id)
{
    std::scoped_lock lock(cmdMutex);
    Command cmd;
    cmd.type = CommandType::RemoveEntity;
    cmd.id = id;
    commandQueue.push(cmd);
}

void World::FlushCommands()
{
    std::scoped_lock lock(cmdMutex);
    while (!commandQueue.empty())
    {
        Command cmd = commandQueue.front();
        commandQueue.pop();

        if (cmd.type == CommandType::AddEntity)
        {
            auto e = std::make_unique<Entity>(cmd.name.c_str());

            // Automatically add Transform Component
            auto t = std::make_unique<Transform>();
            e->AddComponent<Transform>();

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
