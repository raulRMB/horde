//
// Created by Raul on 12/4/2023.
//

#ifndef C_CAMERA_H
#define C_CAMERA_H

#include "util/defines.h"

namespace raylib
{
    struct Camera3D;
}

namespace tZ
{

typedef struct CCamera3D {
    v3 Position;       // Camera position
    v3 Target;         // Camera target it looks-at
    v3 Up;             // Camera up vector (rotation over its axis)
    float Fovy;             // Camera field-of-view apperture in Y (degrees) in perspective, used as near plane width in orthographic
    int Projection;         // Camera projection type: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC

} CCamera3D;

    struct raylib::Camera3D ToRaylibCamera(const tZ::CCamera3D& camera);
}





#endif //C_CAMERA_H
