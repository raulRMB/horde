//
// Created by Raul on 12/4/2023.
//

#include "Camera.h"
#include "raylibEx.h"

namespace tZ
{
raylib::Camera3D ToRaylibCamera(const CCamera3D& camera)
{
    raylib::Camera3D raylibCamera{};
    raylibCamera.position = ToRaylibVector3(camera.Position);
    raylibCamera.target = ToRaylibVector3(camera.Target);
    raylibCamera.fovy = camera.Fovy;
    raylibCamera.up = ToRaylibVector3(camera.Up);
    raylibCamera.projection = camera.Projection;

    return raylibCamera;
}
}