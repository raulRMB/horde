#ifndef UTIL_H
#define UTIL_H

#include <raylib.h>
#include <rcamera.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../../game/Game.h"
#include "../components/Components.h"
#include "primitives/Triangles.h"
#include "primitives/Polygon.h"

namespace Util
{
    static glm::mat4 GLMMat4FromMatrix(const Matrix& matrix);
    Vector3 GetMouseWorldPosition();
    Vector2 GetMouseWorldPosition2D();
    Vector3 RandVec3(int min, int max);
    Color RandColor();
    RayCollision GetMouseCollision();
    Attribute& GetAttribute(AttributesComponent& ac, const std::string& attr);
    void ApplyEffect(AttributesComponent& ac, const Effect& effect);
    Vector2 GetClosetPointOusideTriangle(Vector2 p, const Triangle2D& t);
    bool IsPointInsideTriangle(const Triangle2D& triangle, const Vector2& point);
    Vector2 ClosestPointOnLine(const Vector2& start, const Vector2& end, Vector2 P);
    Vector2 MinimumTranslationVector(const Polygon2D& polygon, const Vector2& point);
}

#endif //UTIL_H
