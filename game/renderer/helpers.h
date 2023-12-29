#ifndef HORDE_HELPERS_H
#define HORDE_HELPERS_H

#include <cstdio>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#include "bx/math.h"

namespace tX {

    bgfx::ShaderHandle loadShader(const char *FILENAME) {
        const char *shaderPath = "";
        switch (bgfx::getRendererType()) {
            case bgfx::RendererType::Noop:
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12:
                shaderPath = "../assets/shaders/examples/dx11/";
                break;
            case bgfx::RendererType::Gnm:
                shaderPath = "../assets/shaders/examples/pssl/";
                break;
            case bgfx::RendererType::Metal:
                shaderPath = "../assets/shaders/examples/metal/";
                break;
            case bgfx::RendererType::OpenGL:
                shaderPath = "../assets/shaders/examples/glsl/";
                break;
            case bgfx::RendererType::OpenGLES:
                shaderPath = "../assets/shaders/examples/essl/";
                break;
            case bgfx::RendererType::Vulkan:
                shaderPath = "../assets/shaders/examples/spirv/";
                break;
        }
        size_t shaderLen = strlen(shaderPath);
        size_t fileLen = strlen(FILENAME);
        char *filePath = (char *) malloc(shaderLen + fileLen + 1);
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
            {-1.0f, 1.0f,  1.0f,  0xff000000},
            {1.0f,  1.0f,  1.0f,  0xff0000ff},
            {-1.0f, -1.0f, 1.0f,  0xff00ff00},
            {1.0f,  -1.0f, 1.0f,  0xff00ffff},
            {-1.0f, 1.0f,  -1.0f, 0xffff0000},
            {1.0f,  1.0f,  -1.0f, 0xffff00ff},
            {-1.0f, -1.0f, -1.0f, 0xffffff00},
            {1.0f,  -1.0f, -1.0f, 0xffffffff}
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

}
#endif //HORDE_HELPERS_H
