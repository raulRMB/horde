#ifndef RAYLIBEX_H
#define RAYLIBEX_H

#include "defines.h"
#include "primitives/Color.h"
#include "components/Rectangle.h"
#include "components/Transform.h"
namespace raylib
{
    #include <raylib.h>
}

namespace tZ
{

struct ParticleInstanceData
{
    raylib::Matrix Transform;
    FColor Color;
};

inline raylib::Vector3 ToRaylibVector3(const v3& vec)
{
    return (raylib::Vector3){vec.x, vec.y, vec.z};
}

inline raylib::Vector2 ToRaylibVector2(const v2& vec)
{
    return (raylib::Vector2){vec.x, vec.y};
}

inline raylib::Rectangle ToRaylibRect(const CRectangle& rect)
{
    return (raylib::Rectangle){rect.X, rect.Y, rect.Width, rect.Height};
}

inline raylib::Color ToRaylibColor(const FColor& color)
{
    return (raylib::Color){color.R, color.G, color.B, color.A};
}

inline raylib::Matrix ToRaylibMatrix(const glm::mat4& glmMatrix)
{
    raylib::Matrix raylibMatrix{};
    raylibMatrix.m0 = glmMatrix[0][0]; raylibMatrix.m1 = glmMatrix[0][1]; raylibMatrix.m2 = glmMatrix[0][2]; raylibMatrix.m3 = glmMatrix[0][3];
    raylibMatrix.m4 = glmMatrix[1][0]; raylibMatrix.m5 = glmMatrix[1][1]; raylibMatrix.m6 = glmMatrix[1][2]; raylibMatrix.m7 = glmMatrix[1][3];
    raylibMatrix.m8 = glmMatrix[2][0]; raylibMatrix.m9 = glmMatrix[2][1]; raylibMatrix.m10 = glmMatrix[2][2]; raylibMatrix.m11 = glmMatrix[2][3];
    raylibMatrix.m12 = glmMatrix[3][0]; raylibMatrix.m13 = glmMatrix[3][1]; raylibMatrix.m14 = glmMatrix[3][2]; raylibMatrix.m15 = glmMatrix[3][3];
    return raylibMatrix;
}

void DrawMeshInstancedWithColor(raylib::Mesh mesh, raylib::Material material, const ParticleInstanceData* instanceData,  int instances);
v2 GetMousePositionGLM();

}


#endif //RAYLIBEX_H
