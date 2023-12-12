#ifndef HORDE_PARTICLEDRAWING_H
#define HORDE_PARTICLEDRAWING_H

#include "systems/System.h"
namespace raylib
{
#include "raylib.h"
}

#define MAX_INSTANCES 100000

namespace tZ
{
class SParticleDrawing : public DrawingSystem
{
    raylib::Matrix* InstanceTransforms = (raylib::Matrix*)RL_CALLOC(MAX_INSTANCES, sizeof(raylib::Matrix));
    raylib::Mesh ParticleMesh{};
    raylib::Material ParticleMaterial{};
    raylib::Shader ParticleShader{};

public:
    void Init() override;
    void Draw() override;

    ~SParticleDrawing() override;
};
}

#endif //HORDE_PARTICLEDRAWING_H
