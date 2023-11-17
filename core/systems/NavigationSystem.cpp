//
// Created by Raul on 11/17/2023.
//

#include "NavigationSystem.h"
#include "../Components.h"
#include "../navigation/Navigation.h"
#include "../../game/Game.h"

void NavigationSystem::SetMapTriangles(const std::vector<Navigation::TriangleNode>& mapTriangles)
{
    MapTriangles = mapTriangles;
}

void NavigationSystem::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    const auto followEntities = registry.view<FollowComponent, Transform>();
    for(const entt::entity& ent : followEntities)
    {
        std::vector<Navigation::TriangleNode*> path;
        FollowComponent& follow = GetComponent<FollowComponent>(ent);

        Vector2 startPoint = {GetComponent<Transform>(ent).translation.x, GetComponent<Transform>(ent).translation.z};
        std::vector<Edge2D> portals;
        Navigation::AStar(startPoint, follow.TargetPos, path, portals, MapTriangles);

        std::vector<Vector2>& stringPath = follow.StringPath;
        stringPath = Navigation::StringPull(portals, startPoint, follow.TargetPos);

        follow.TargetPos = stringPath[follow.Index];
    }
}
