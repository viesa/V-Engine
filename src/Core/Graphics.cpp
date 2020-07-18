#include "Graphics.h"

#include <GLFW/glfw3.h>

Graphics::Graphics(Window &window)
        : m_wnd(window)
{

}

void Graphics::DrawTriangle()
{

}

void Graphics::BeginFrame(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::EndFrame()
{
    glfwSwapBuffers(m_wnd.GetCoreWindow());
    glfwPollEvents();
}
