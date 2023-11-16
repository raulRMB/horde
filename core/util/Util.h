#ifndef UTIL_H
#define UTIL_H

#include <raylib.h>
#include <rcamera.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../../game/Game.h"

glm::mat4 GLMMat4FromMatrix(const Matrix& matrix);

inline Vector3 GetMouseWorldPosition(bool atZeroZ = false)
{
    int x, y;
    x = GetMousePosition().x;
    y = GetMousePosition().y;

    double x_ndc = 2.f * (float)x / 800 - 1.f;
    double y_ndc = 2.f * (float)y / 600 - 1.f;

    Camera3D camera = Game::Instance().GetActiveCamera();
    glm::mat4 view = GLMMat4FromMatrix(GetCameraViewMatrix(&camera));
    glm::mat4 projection = GLMMat4FromMatrix(GetCameraProjectionMatrix(&camera, 800.f / 600.f));
    glm::mat4 viewProjectionInverse = projection * view;
    viewProjectionInverse = glm::inverse(viewProjectionInverse);
    glm::vec4 worldSpacePosition(x_ndc, y_ndc, atZeroZ ? 0.f : 1.f, 1.f);
    auto world = viewProjectionInverse * worldSpacePosition;
    world /= world.w;
    return {world.x, world.y, world.z};
}



#endif //UTIL_H
