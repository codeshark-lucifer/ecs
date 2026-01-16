#pragma once
#include <SDL3/SDL.h>
#include <functional>

class Platform
{
public:
    Platform() = default;
    ~Platform();

    bool Initialize(const int &width, const int &height, const char *title);
    bool ShouldClose() const;

    void PollEvents();
    void SwapBuffers();

    void ListenEvent(const std::function<void(int, int)> &func);
    float GetDeltaTime() const;
private:
    bool running = false;
    std::function<void(int, int)> callback; // on resize

    SDL_Event event;
    SDL_Window *window;
    SDL_GLContext gl_context;

    float deltaTime;
    float lastTime;
};
