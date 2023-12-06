#include "Transform.h"
#include <glm/gtx/quaternion.hpp>

namespace tZ
{

CTransform::CTransform(const v3& position, const v3& scale, const v4& vRotation, const quat& rotation)
    : Position(position), Scale(scale), VRotation(vRotation), Rotation(rotation), Matrix(m4(1.f))
{
    UpdateMatrix();
}

void CTransform::UpdateMatrix()
{
    Matrix = glm::translate(m4(1.f), Position) * glm::toMat4(Rotation) * glm::scale(m4(1.f), Scale);
}
    
}
