#include "CTransform.h"
#include "glm/gtx/quaternion.hpp"

namespace tX
{

CTransform::CTransform(const v3& position, const v3& scale, const quat& rotation)
    : Position(position), Scale(scale), Rotation(rotation), Matrix(m4(1.f)), CComponent("Transform")
{
    UpdateMatrix();
}

void CTransform::UpdateMatrix()
{
    Matrix = glm::translate(m4(1.f), Position) * glm::toMat4(Rotation) * glm::scale(m4(1.f), Scale);
}
    
}
