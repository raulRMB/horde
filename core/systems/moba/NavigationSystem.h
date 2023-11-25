//
// Created by Raul on 11/17/2023.
//

#ifndef NAVIGATIONSYSTEM_H
#define NAVIGATIONSYSTEM_H

#include "systems/System.h"
#include "navigation/Navigation.h"

class NavigationSystem final : public UpdateSystem
{
private:
    std::vector<Navigation::TriangleNode> NavMesh{};
    std::vector<Vector2> Points;
public:
    const std::vector<Navigation::TriangleNode>& GetMapTriangles() { return NavMesh; }
    void Update(float deltaSeconds) override;
    void LoadNavMesh();
    void SaveNavMesh();
    bool IsValidPoint(const Vector2& point);
};



#endif //NAVIGATIONSYSTEM_H
