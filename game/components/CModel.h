#ifndef HORDE_CMODEL_H
#define HORDE_CMODEL_H
#include "tX_Component.h"
#include "util/defines.h"

namespace tX
{

struct CModel : CComponent
{
    //raylib::Model Model;
    f32 Scale;
    bool bHidden;
//    CModel(raylib::Model model = raylib::LoadModelFromMesh(raylib::GenMeshCube(1.0f, 1.0f, 1.0f)),
//           f32 scale = 1.f, bool hidden = false)
//           :
//           Model(model), Scale(scale), bHidden(hidden), CComponent("Model") {}
};

}

#endif //HORDE_CMODEL_H
