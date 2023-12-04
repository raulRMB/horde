#ifndef C_TRANSFORM_H
#define C_TRANSFORM_H

#include "defines.h"

namespace tZ
{

typedef struct CTransform
{
    v3 Position;
    v3 Scale;
    quat Rotation;
    m4 Matrix;

    CTransform() = default;
    CTransform(const v3& position, const v3& scale, const quat& rotation);
protected:
    void UpdateMatrix();

} CTransform;

}

#endif //C_TRANSFORM_H
