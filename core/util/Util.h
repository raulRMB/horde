#ifndef UTIL_H
#define UTIL_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "../../game/Game.h"

namespace tZ
{
    struct CPhysics2D;
    struct FAttribute;
    struct CAttributes;
    struct FRayCollision;
    struct CTransform;
    struct Polygon2D;
    struct Triangle2D;
}

namespace tZ
{

namespace Util
{
    v3 GetMouseWorldPosition();
    v2 GetMouseWorldPosition2D();
    v3 RandVec3(int min, int max);
    FColor RandColor();
    FRayCollision GetMouseCollision();
    FAttribute* GetAttribute(CAttributes& ac, const std::string& attr);
    v2 GetClosetPointOusideTriangle(v2 p, const Triangle2D& t);
    bool IsPointInsideTriangle(const Triangle2D& triangle, const v2& point);
    v2 ClosestPointOnLine(const v2& start, const v2& end, v2 P);
    v2 MinimumTranslationVector(const Polygon2D& polygon, const v2& point);

    bool Check2DCollision(CPhysics2D &x, CTransform &xT, CPhysics2D &y, CTransform &yT);
}

}

#endif //UTIL_H
