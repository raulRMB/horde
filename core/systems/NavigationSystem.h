//
// Created by Raul on 11/17/2023.
//

#ifndef NAVIGATIONSYSTEM_H
#define NAVIGATIONSYSTEM_H

#include "System.h"
#include "../navigation/Navigation.h"

class NavigationSystem final : public UpdateSystem
{
private:
    std::vector<Navigation::TriangleNode> MapTriangles{};
public:
    void SetMapTriangles(const std::vector<Navigation::TriangleNode>& mapTriangles);
    const std::vector<Navigation::TriangleNode>& GetMapTriangles() { return MapTriangles; }
    void Update(float deltaSeconds) override;
};



#endif //NAVIGATIONSYSTEM_H
