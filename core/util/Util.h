#ifndef UTIL_H
#define UTIL_H

#include <raylib.h>
#include <rcamera.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../../game/Game.h"

namespace Util
{
    static glm::mat4 GLMMat4FromMatrix(const Matrix& matrix);
    Vector3 GetMouseWorldPosition();
    Vector2 GetMouseWorldPosition2D();
    Vector3 RandVec3(int min, int max);
    Color RandColor();
    RayCollision GetMouseCollision();
}

#endif //UTIL_H
