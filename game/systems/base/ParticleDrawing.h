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

struct ParticleInstanceData;

class SParticleDrawing : public DrawingSystem
{
    ParticleInstanceData* InstanceData{};
    i32 InstanceLocation{};
    raylib::Mesh ParticleMesh{};
    raylib::Material ParticleMaterial{};
    raylib::Shader ParticleShader{};


public:
    void Init() override;
    void Draw() override;

    SParticleDrawing();

    ~SParticleDrawing() override;
};
}

#endif //HORDE_PARTICLEDRAWING_H
