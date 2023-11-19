#include "Util.h"

glm::mat4 Util::GLMMat4FromMatrix(const Matrix& matrix)
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

    return result;
}

Vector3 Util::RandVec3(int min, int max) {
    return Vector3 {
            (float)GetRandomValue(min, max),
            (float)GetRandomValue(min, max),
            (float)GetRandomValue(min, max)
    };
}

RayCollision Util::GetMouseCollision() {
    Ray ray = GetMouseRay(GetMousePosition(), Game::Instance().GetActiveCamera());
    Vector3 TopLeft = {-1000.0f, 0.0f, -1000.0f};
    Vector3 TopRight = {1000.0f, 0.0f, -1000.0f};
    Vector3 BottomLeft = {-1000.0f, 0.0f, 1000.0f};
    Vector3 BottomRight = {1000.0f, 0.0f, 1000.0f};
    return GetRayCollisionQuad(ray, TopRight, TopLeft, BottomLeft, BottomRight);
}

Color Util::RandColor() {
    return Color {
            (unsigned char) GetRandomValue(1, 255),
            (unsigned char) GetRandomValue(1, 255),
            (unsigned char) GetRandomValue(1, 255),
            255
    };
}

Vector3 Util::GetMouseWorldPosition()
{
    const Ray ray = GetMouseRay(GetMousePosition(), Game::Instance().GetActiveCamera());
    constexpr Vector3 TopLeft = {-1000.0f, 0.0f, -1000.0f};
    constexpr Vector3 TopRight = {1000.0f, 0.0f, -1000.0f};
    constexpr Vector3 BottomLeft = {-1000.0f, 0.0f, 1000.0f};
    constexpr Vector3 BottomRight = {1000.0f, 0.0f, 1000.0f};
    const RayCollision Collision = GetRayCollisionQuad(ray, TopRight, TopLeft , BottomLeft, BottomRight);
    return Collision.point;
}

Vector2 Util::GetMouseWorldPosition2D()
{
    const Vector3 mousePos3d = GetMouseWorldPosition();
    return {mousePos3d.x, mousePos3d.z};
}