#include "Renderer.h"
#include "helpers.h"

namespace tX {

    Renderer::Renderer() {}

    Renderer &Renderer::Instance() {
        static Renderer instance;
        return instance;
    }

    bool Renderer::Init() {
        if (!glfwInit())
            return false;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        Instance().window = glfwCreateWindow(1024, 768, "Horde", nullptr, nullptr);
        if (!Instance().window)
            return false;
        bgfx::renderFrame();
        bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
        init.platformData.ndt = glfwGetX11Display();
	init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
        init.platformData.nwh = glfwGetCocoaWindow(Instance().window);
#elif BX_PLATFORM_WINDOWS
        init.platformData.nwh = glfwGetWin32Window(window);
#endif
        glfwGetWindowSize(Instance().window, &Instance().width, &Instance().height);
        init.resolution.width = (uint32_t)Instance().width;
        init.resolution.height = (uint32_t)Instance().height;
        init.resolution.reset = BGFX_RESET_VSYNC;
        if (!bgfx::init(init))
            return false;

        bgfx::setViewClear(Instance().kClearView, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH);
        bgfx::setViewRect(Instance().kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        Instance().pcvDecl.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();
        Instance().vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), Instance().pcvDecl);
        Instance().ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
        Instance().vsh = loadShader("vs_cubes.bin");
        Instance().fsh = loadShader("fs_cubes.bin");

        Instance().program = bgfx::createProgram(Instance().vsh, Instance().fsh, true);
        if (!bgfx::isValid(Instance().program)) {
            fprintf(stderr, "Failed to create shader program\n");
            return false;
        }
        return true;
    }

    void Renderer::BeginDraw() {
        bgfx::touch(Instance().kClearView);
    }

    void Renderer::EndDraw() {
        bgfx::setVertexBuffer(0, Instance().vbh);
        bgfx::setIndexBuffer(Instance().ibh);

        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0, Instance().program);
        bgfx::frame();
    }

    void Renderer::HandleWindow() {
        glfwPollEvents();
        int oldWidth = Instance().width, oldHeight = Instance().height;
        glfwGetWindowSize(Instance().window, &Instance().width, &Instance().height);
        if (Instance().width != oldWidth || Instance().height != oldHeight) {
            bgfx::reset((uint32_t)Instance().width, (uint32_t)Instance().height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(Instance().kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        }
    }

    void Renderer::Clean() {
        bgfx::shutdown();
        glfwTerminate();
    }

}