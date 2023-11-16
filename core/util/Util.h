#ifndef UTIL_H
#define UTIL_H

#include <raylib.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/detail/func_matrix.inl>
#include "../game/Game.h"

glm::mat4 GLMMat4FromMatrix(const Matrix& matrix)
{
    glm::mat4 result{};
    result[0][0] = matrix.m0;
    result[0][1] = matrix.m1;
    result[0][2] = matrix.m2;
    result[0][3] = matrix.m3;
    result[1][0] = matrix.m4;
    result[1][1] = matrix.m5;
    result[1][2] = matrix.m6;
    result[1][3] = matrix.m7;
    result[2][0] = matrix.m8;
    result[2][1] = matrix.m9;
    result[2][2] = matrix.m10;
    result[2][3] = matrix.m11;
    result[3][0] = matrix.m12;
    result[3][1] = matrix.m13;
    result[3][2] = matrix.m14;
    result[3][3] = matrix.m15;
}

inline Vector3 GetMouseWorldPosition(bool atZeroZ = false)
{
    int x, y;
    x = GetMousePosition().x;
    y = GetMousePosition().y;

    double x_ndc = 2.f * (float)x / GetWindowScaleDPI().x - 1.f;
    double y_ndc = 2.f * (float)y / GetWindowScaleDPI().y - 1.f;

    glm::mat4 viewProjectionInverse = GLMMat4FromMatrix(GetCameraMatrix(Game::Instance().GetActiveCamera()));
    viewProjectionInverse = glm::inverse(viewProjectionInverse);
    glm::vec4 worldSpacePosition(x_ndc, y_ndc, atZeroZ ? 0.f : 1.f, 1.f);
    auto world = viewProjectionInverse * worldSpacePosition;
    world /= world.w;
    return {world.x, world.y, world.z};
}



#endif //UTIL_H
