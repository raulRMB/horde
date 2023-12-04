//
// Created by Raul on 12/4/2023.
//

#include "Camera.h"
#include "raylibEx.h"

Camera3D ToRaylibCamera(const tZ::CCamera3D& camera)
{
    Camera3D raylibCamera{};
    raylibCamera.position = tZ::ToRaylibVector3(camera.Position);
    raylibCamera.target = tZ::ToRaylibVector3(camera.Target);
    raylibCamera.fovy = camera.Fovy;
    raylibCamera.up = tZ::ToRaylibVector3(camera.Up);
    raylibCamera.projection = camera.Projection;

    return raylibCamera;
}