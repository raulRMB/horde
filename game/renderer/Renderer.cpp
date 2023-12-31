#include "Renderer.h"
#include "helpers.h"
#include <iostream>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#include "tiny_gltf.h"
#include "util/defines.h"
#include "graphics/Texture2D.h"

namespace tX {

    Renderer::Renderer() {}

    Renderer &Renderer::Instance() {
        static Renderer instance;
        return instance;
    }

    struct PosNormalVertex
    {
        float x = 0.f, y = 0.f, z = 0.0f;
        float u = 0.f, v = 0.f;
    };

    bgfx::UniformHandle s_texColor;
    FTexture2D Texture = FTexture2D();
    bgfx::TextureHandle m_textureColor;

    std::vector<PosNormalVertex> vertices;
    std::vector<uint16_t> indices;

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
        init.platformData.nwh = glfwGetWin32Window(GetWindow());
#endif
        glfwGetWindowSize(Instance().window, &Instance().width, &Instance().height);
        init.resolution.width = (uint32_t)Instance().width;
        init.resolution.height = (uint32_t)Instance().height;
        init.resolution.reset = BGFX_RESET_VSYNC;
        if (!bgfx::init(init))
            return false;

        bgfx::setViewClear(Instance().kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
        bgfx::setViewRect(Instance().kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
//        Instance().pcvDecl.begin()
//                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
//                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
//                .end();
//        Instance().vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), Instance().pcvDecl);
//        Instance().ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
        Instance().vsh = loadShader("vs_model.bin");
        Instance().fsh = loadShader("fs_model.bin");

        s_texColor = bgfx::createUniform("s_texColor",  bgfx::UniformType::Sampler);
        m_textureColor = Texture.Load("../assets/textures/ninja.png");

        Instance().program = bgfx::createProgram(Instance().vsh, Instance().fsh, true);
        if (!bgfx::isValid(Instance().program)) {
            fprintf(stderr, "Failed to create shader program\n");
            return false;
        }

        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err;
        std::string warn;

        bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, "../assets/models/anim.glb");
        if (!warn.empty()) {
            printf("Warn: %s\n", warn.c_str());
        }

        if (!err.empty()) {
            printf("Err: %s\n", err.c_str());
        }

        if (!ret) {
            printf("Failed to parse glTF\n");
        }
        else
        {
            printf("Loaded glTF\n");
        }

        // Extract vertex and index data from glTF

        for(const auto& mesh : model.meshes)
        {
            for (const auto &primitive: mesh.primitives)
            {
                const tinygltf::Accessor &accessor = model.accessors[primitive.indices];
                const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

                const void *indexData = &buffer.data[bufferView.byteOffset + accessor.byteOffset];
                const uint32_t indexCount = static_cast<uint32_t>(accessor.count);

                indices.resize(indexCount);
                memcpy(indices.data(), indexData, indexCount * sizeof(uint16_t));

                const tinygltf::Accessor &positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
                const tinygltf::BufferView &positionView = model.bufferViews[positionAccessor.bufferView];
                const tinygltf::Buffer &positionBuffer = model.buffers[positionView.buffer];

                const void *positionData = &positionBuffer.data[positionView.byteOffset + positionAccessor.byteOffset];
                const uint32_t vertexCount = static_cast<uint32_t>(positionAccessor.count);

                vertices.resize(vertexCount);

                const tinygltf::Accessor &uvAccessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
                const tinygltf::BufferView &uvView = model.bufferViews[uvAccessor.bufferView];
                const tinygltf::Buffer &uvBuffer = model.buffers[uvView.buffer];

                const void *uvData = &uvBuffer.data[uvView.byteOffset + uvAccessor.byteOffset];

                u32 i = 0;
                for(auto& vertex : vertices)
                {
                    vertex.x = ((float*)positionData)[i + 0] * 0.01f;
                    vertex.y = ((float*)positionData)[i + 1] * 0.01f;
                    vertex.z = ((float*)positionData)[i + 2] * 0.01f;

                    i += 3;
                }

                i = 0;
                for(auto& vertex : vertices)
                {
                    vertex.u = ((float*)uvData)[i + 0];
                    vertex.v = ((float*)uvData)[i + 1];
                    i += 2;
                }

//                std::cout << i << std::endl;
//                memcpy(vertices.data(), positionData, vertexCount * sizeof(PosNormalVertex));
            }
        }


        Instance().pcvDecl.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .end();

//        // Create BGFX buffers
        Instance().vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices.data(), vertices.size() * sizeof(PosNormalVertex)), Instance().pcvDecl);
        Instance().ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices.data(), indices.size() * sizeof(uint16_t)));

        return true;
    }

    void Renderer::BeginDraw() {
        bgfx::touch(Instance().kClearView);
    }

    void Renderer::EndDraw() {

        bgfx::setVertexBuffer(0, Instance().vbh);
        bgfx::setIndexBuffer(Instance().ibh);

        bgfx::setTexture(0, s_texColor, m_textureColor);

        bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_CULL_CCW);
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