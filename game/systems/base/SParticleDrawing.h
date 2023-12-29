#ifndef HORDE_SPARTICLEDRAWING_H
#define HORDE_SPARTICLEDRAWING_H

#include "systems/System.h"

#define MAX_INSTANCES 100000

namespace tX
{

struct ParticleInstanceData;

class SParticleDrawing : public DrawingSystem
{
    ParticleInstanceData* InstanceData{};
    i32 InstanceLocation{};
//    raylib::Mesh ParticleMesh{};
//    raylib::Material ParticleMaterial{};
//    raylib::Shader ParticleShader{};


public:
    void Init() override;
    void Draw() override;

    SParticleDrawing();

    ~SParticleDrawing() override;
};
}

#endif //HORDE_SPARTICLEDRAWING_H
