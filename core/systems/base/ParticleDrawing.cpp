#include "ParticleDrawing.h"
#include "components/Particle.h"
#include "raylibEx.h"
namespace raylib
{

#include "raymath.h"
}
namespace tZ
{
void SParticleDrawing::Draw()
{
    u32 idx = 0;
    auto& registry = Game::GetRegistry();

    const CCamera3D& camera = Game::Instance().GetActiveCamera();
    float cameraPos[3] = { camera.Position.x, camera.Position.y, camera.Position.z };
    SetShaderValue(ParticleShader, ParticleShader.locs[raylib::SHADER_LOC_VECTOR_VIEW], cameraPos, raylib::SHADER_UNIFORM_VEC3);
    for(const entt::entity& entity : registry.view<CParticle, CTransform>())
    {
        if(idx >= MAX_INSTANCES)
            break;

        CTransform& transform = registry.get<CTransform>(entity);
        m4 mat = glm::translate(glm::mat4(1.0f), transform.Position);
        mat = glm::rotate(mat, glm::radians(transform.VRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        mat = glm::rotate(mat, glm::radians(transform.VRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        mat = glm::rotate(mat, glm::radians(transform.VRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        mat = glm::scale(mat, transform.Scale);
        InstanceTransforms[idx] = ToRaylibMatrix(mat);
        idx++;
    }
    if(idx > 0)
        raylib::DrawMeshInstanced(ParticleMesh, ParticleMaterial, InstanceTransforms, idx < MAX_INSTANCES ? (i32)idx : MAX_INSTANCES);
}

    void SParticleDrawing::Init()
    {
        System::Init();
        ParticleMesh = raylib::GenMeshPlane(1.f, 1.f, 1.f, 1.f);

        ParticleShader = raylib::LoadShader("../assets/shaders/instanced_test.vs", "../assets/shaders/base.fs");
        ParticleShader.locs[raylib::SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(ParticleShader, "viewPos");
        ParticleShader.locs[raylib::SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(ParticleShader, "instanceTransform");

        ParticleMaterial = raylib::LoadMaterialDefault();
        ParticleMaterial.shader = ParticleShader;
        ParticleMaterial.maps[raylib::MATERIAL_MAP_DIFFUSE].texture = raylib::LoadTexture("../assets/textures/particle.png");
    }

    SParticleDrawing::~SParticleDrawing()
    {
        RL_FREE(InstanceTransforms);
    }
}
