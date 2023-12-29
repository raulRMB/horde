#ifndef HORDE_RENDERER_H
#define HORDE_RENDERER_H

#include <cstdio>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#include "bx/math.h"

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

namespace tX
{

    class Renderer {

        Renderer();

        static Renderer& Instance();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        int width, height;
        int counter = 0;
        GLFWwindow* window;
        const bgfx::ViewId kClearView = 0;
        bgfx::VertexLayout pcvDecl;
        bgfx::VertexBufferHandle vbh;
        bgfx::IndexBufferHandle ibh;
        bgfx::ShaderHandle vsh;
        bgfx::ShaderHandle fsh;
        bgfx::ProgramHandle program;

    public:
        static bool Init();
        static GLFWwindow* GetWindow() { return Instance().window; };
        static void BeginDraw();
        static void EndDraw();
        static int GetWidth() { return Instance().width; };
        static int GetHeight() { return Instance().height; };
        static void HandleWindow();
        static bool WindowShouldClose() { return glfwWindowShouldClose(Instance().window); };
        static void Clean();
    };

}


#endif //HORDE_RENDERER_H
