#include "ParticleDrawing.h"
#include "components/Particle.h"
#include "util/raylibEx.h"
#include "glm/gtc/quaternion.hpp"

namespace raylib
{
#include "raymath.h"
#include "rlgl.h"
}
namespace tZ
{
void SParticleDrawing::Draw()
{
    u32 idx = 0;
    entt::registry& registry = Game::GetRegistry();

    const CCamera3D& camera = Game::Instance().GetActiveCamera();
    const float cameraPos[3] = { camera.Position.x, camera.Position.y, camera.Position.z };
    SetShaderValue(ParticleShader, ParticleShader.locs[raylib::SHADER_LOC_VECTOR_VIEW], cameraPos, raylib::SHADER_UNIFORM_VEC3);

    for(const entt::entity& entity : registry.view<CParticle, CTransform>())
    {
        if(idx >= MAX_INSTANCES)
            break;

        CTransform& transform = registry.get<CTransform>(entity);
        m4 mat = glm::translate(glm::mat4(1.0f), transform.Position);
        mat = glm::mat4_cast(transform.Rotation) * mat;
        mat = glm::scale(mat, transform.Scale);
        InstanceData[idx].Transform = ToRaylibMatrix(mat);
        InstanceData[idx].Color = GetComponent<CParticle>(entity).Color;
        idx++;
    }
    if(idx > 0)
        DrawMeshInstancedWithColor(ParticleMesh, ParticleMaterial, InstanceData, idx < MAX_INSTANCES ? static_cast<i32>(idx) : MAX_INSTANCES);
}

SParticleDrawing::SParticleDrawing()
{
    InstanceData = (ParticleInstanceData*)RL_CALLOC(MAX_INSTANCES, sizeof(ParticleInstanceData));
}

void SParticleDrawing::Init()
    {
        System::Init();
        ParticleMesh = raylib::GenMeshPlane(1.f, 1.f, 1.f, 1.f);

        ParticleShader = raylib::LoadShader("../assets/shaders/instanced_test.vert", "../assets/shaders/base.frag");

        ParticleShader.locs[raylib::SHADER_LOC_MATRIX_MVP] = GetShaderLocation(ParticleShader, "mvp");
        ParticleShader.locs[raylib::SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(ParticleShader, "viewPos");
        ParticleShader.locs[raylib::SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(ParticleShader, "instanceTransform");
        ParticleShader.locs[raylib::SHADER_LOC_COLOR_DIFFUSE] = GetShaderLocationAttrib(ParticleShader, "instanceColor");

        ParticleMaterial = raylib::LoadMaterialDefault();
        ParticleMaterial.shader = ParticleShader;
        ParticleMaterial.maps[raylib::MATERIAL_MAP_DIFFUSE].texture = raylib::LoadTexture("../assets/textures/particle.png");
    }

    SParticleDrawing::~SParticleDrawing()
    {
        RL_FREE(InstanceData);
    }
}
