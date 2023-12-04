#ifndef F_MODEL_H
#define F_MODEL_H

#include "defines.h"

namespace tZ
{

typedef struct FModel
{
    m4 transform;       // Local transform matrix

    int meshCount;          // Number of meshes
    int materialCount;      // Number of materials
    Mesh *meshes;           // Meshes array
    Material *materials;    // Materials array
    int *meshMaterial;      // Mesh material number

    // Animation data
    int boneCount;          // Number of bones
    BoneInfo *bones;        // Bones information (skeleton)
    Transform *bindPose;    // Bones base transformation (pose)
} FModel;

}

#endif //F_MODEL_H
