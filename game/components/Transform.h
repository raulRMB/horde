#ifndef C_TRANSFORM_H
#define C_TRANSFORM_H

#include "util/defines.h"

namespace tZ
{

typedef struct CTransform
{
    v3 Position;
    v3 Scale;
    quat Rotation;
    m4 Matrix;

    CTransform(const v3& position = v3(0.f), const v3& scale = v3(1.f), const quat& rotation = glm::identity<quat>());
protected:
    void UpdateMatrix();

} CTransform;

}

#endif //C_TRANSFORM_H
