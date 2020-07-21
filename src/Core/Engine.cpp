#include "Engine.h"

#include <cstdio>

#include <GLFW/glfw3.h>

void Engine::Initialize(Engine::Subsystem subsystem)
{
    BindDefaultErrorCallback();
    glfwInit();
}

void Engine::Uninitialize(Engine::Subsystem subsystem)
{
    glfwTerminate();
}
void Engine::BindErrorCallback(GLFWerrorfun callback)
{
    glfwSetErrorCallback(callback);
}

void Engine::BindDefaultErrorCallback()
{
    glfwSetErrorCallback([](int error, const char *desc)
                         {
                             printf("Unexcepted error [%d] Description: %s", error, desc);
                         });
}

void Engine::UnbindErrorCallback()
{
    glfwSetErrorCallback(nullptr);
}
