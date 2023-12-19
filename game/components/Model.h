#ifndef HORDE_MODEL_H
#define HORDE_MODEL_H

namespace raylib
{
#include "raylib.h"
}
#include "TechXComponent.h"
#include "util/defines.h"

namespace tZ
{

struct CModel : CComponent
{
    raylib::Model Model;
    f32 Scale;
    bool bHidden;
    CModel(raylib::Model model = raylib::LoadModelFromMesh(raylib::GenMeshCube(1.0f, 1.0f, 1.0f)),
           f32 scale = 1.f, bool hidden = false)
           :
           Model(model), Scale(scale), bHidden(hidden), CComponent("Model") {}
};

}

#endif //HORDE_MODEL_H
