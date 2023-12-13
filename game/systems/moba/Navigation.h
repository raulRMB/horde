//
// Created by Raul on 11/17/2023.
//

#ifndef NAVIGATIONSYSTEM_H
#define NAVIGATIONSYSTEM_H

#include "systems/System.h"
#include "navigation/Navigation.h"

namespace tZ
{

class SNavigation final : public UpdateSystem
{
private:
    std::vector<Navigation::TriangleNode> NavMesh{};
    std::vector<v2> Points;
public:
    const std::vector<Navigation::TriangleNode>& GetMapTriangles() { return NavMesh; }
    void Update(float deltaSeconds) override;
    void LoadNavMesh();
    void SaveNavMesh();
    bool IsValidPoint(const v2& point);
};

}

#endif //NAVIGATIONSYSTEM_H
