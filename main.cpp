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

bgfx::ShaderHandle loadShader(const char *FILENAME) {
    const char* shaderPath = "";
    switch(bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: shaderPath = "../assets/shaders/examples/dx11/";  break;
        case bgfx::RendererType::Gnm:        shaderPath = "../assets/shaders/examples/pssl/";  break;
        case bgfx::RendererType::Metal:      shaderPath = "../assets/shaders/examples/metal/"; break;
        case bgfx::RendererType::OpenGL:     shaderPath = "../assets/shaders/examples/glsl/";  break;
        case bgfx::RendererType::OpenGLES:   shaderPath = "../assets/shaders/examples/essl/";  break;
        case bgfx::RendererType::Vulkan:     shaderPath = "../assets/shaders/examples/spirv/"; break;
    }
    size_t shaderLen = strlen(shaderPath);
    size_t fileLen = strlen(FILENAME);
    char *filePath = (char *)malloc(shaderLen + fileLen + 1);
    strcpy(filePath, shaderPath);
    strcpy(&filePath[shaderLen], FILENAME);
    FILE *file = fopen(filePath, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);
    return bgfx::createShader(mem);
}

struct PosColorVertex {
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cubeVertices[] = {
        {-1.0f,  1.0f,  1.0f, 0xff000000 },
        { 1.0f,  1.0f,  1.0f, 0xff0000ff },
        {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
        { 1.0f, -1.0f,  1.0f, 0xff00ffff },
        {-1.0f,  1.0f, -1.0f, 0xffff0000 },
        { 1.0f,  1.0f, -1.0f, 0xffff00ff },
        {-1.0f, -1.0f, -1.0f, 0xffffff00 },
        { 1.0f, -1.0f, -1.0f, 0xffffffff }
};

static const uint16_t cubeTriList[] = {
        0, 1, 2,
        1, 3, 2,
        4, 6, 5,
        5, 6, 7,
        0, 2, 4,
        4, 2, 6,
        1, 5, 3,
        5, 7, 3,
        0, 4, 1,
        4, 5, 1,
        2, 3, 6,
        6, 3, 7,
};

static bool init() {
    if (!glfwInit())
        return false;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(1024, 768, "Horde", nullptr, nullptr);
    if (!window)
        return false;
    bgfx::renderFrame();
    bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
	init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
#endif
    glfwGetWindowSize(window, &width, &height);
    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init))
        return false;

    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    pcvDecl.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl);
    ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
    vsh = loadShader("vs_cubes.bin");
    fsh = loadShader("fs_cubes.bin");

    program = bgfx::createProgram(vsh, fsh, true);
    if (!bgfx::isValid(program)) {
        fprintf(stderr, "Failed to create shader program\n");
        return false;
    }
    return true;
}

static void draw() {
    bgfx::touch(kClearView);

    const bx::Vec3 at = {0.0f, 0.0f,  0.0f};
    const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
    float view[16];
    bx::mtxLookAt(view, eye, at);
    float proj[16];
    bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);

    float mtx[16];
    bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
    bgfx::setTransform(mtx);

    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);

    bgfx::setState(BGFX_STATE_DEFAULT);
    bgfx::submit(0, program);
    bgfx::frame();
}

static void cleanup() {
    bgfx::shutdown();
    glfwTerminate();
}

static void handleWindow() {
    glfwPollEvents();
    int oldWidth = width, oldHeight = height;
    glfwGetWindowSize(window, &width, &height);
    if (width != oldWidth || height != oldHeight) {
        bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    }
}

int main(int argc, char **argv) {
    if(!init()) {
        return 0;
    }
    while (!glfwWindowShouldClose(window)) {
        handleWindow();
        draw();
        counter++;
    }
    cleanup();
    return 0;
}