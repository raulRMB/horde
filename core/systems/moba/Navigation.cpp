//
// Created by Raul on 11/17/2023.
//

#include "Navigation.h"

#include <fstream>
#include <sstream>

#include "components/Follow.h"
#include "components/Transform.h"
#include "util/Util.h"

namespace tZ
{

void SNavigation::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    const auto followEntities = registry.view<CFollow, CTransform>();
    for(const entt::entity& followEntity : followEntities)
    {
        CFollow& followComponent = GetComponent<CFollow>(followEntity);
        EFollowState& bFollowing = followComponent.FollowState;
        if(bFollowing != EFollowState::Dirty)
            continue;

        std::vector<v2>& stringPath = followComponent.StringPath;
        stringPath.clear();
        v2& goal = followComponent.Goal;

        v2& targetPos = followComponent.TargetPos;
        unsigned int& followIndex = followComponent.Index;
        v2 startPoint = {GetComponent<CTransform>(followEntity).Position.x, GetComponent<CTransform>(followEntity).Position.z};

        std::vector<Edge2D> portals;
        std::vector<Navigation::TriangleNode*> path;
        Navigation::AStar(startPoint, goal, path, portals, NavMesh);
        stringPath = Navigation::StringPull(portals, startPoint, goal);

        targetPos = stringPath[followIndex];
        followIndex = 1;
        bFollowing = EFollowState::Following;
    }
}

void SNavigation::LoadNavMesh()
{
    std::ifstream file;
    file.open("../assets/save.txt");
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file, line) && line != "TRIANGLES")
        {
            std::stringstream ss(line);
            float x, y;
            ss >> x >> y;
            Points.push_back({x, y});
        }

        NavMesh = Navigation::BowyerWatson(Points);

        while(std::getline(file, line))
        {
            std::stringstream ss(line);
            unsigned int idx;
            bool blocked;
            ss >> idx >> blocked;
            NavMesh[idx].SetBlocked(blocked);
        }

        file.close();

        for(const Navigation::TriangleNode& graphTriangle : NavMesh)
        {
            auto e = CreateEntity();
            CTransform transform;
            transform.Position = v3{graphTriangle.GetCircumCenter().x, 0.f, graphTriangle.GetCircumCenter().y};
            AddComponent(e, transform);
            if(graphTriangle.IsBlocked())
            {
                Triangle2D triangle = graphTriangle.GetTriangle();
                triangle.color = FColor(0xFF0000FF);
                AddComponent(e, triangle);
            }
        }
    }
}

void SNavigation::SaveNavMesh()
{
    std::ofstream file;
    file.open("../assets/save.txt");
    for(const v2& point : Points)
    {
        file << point.x << " " << point.y << "\n";
    }

    file << "TRIANGLES\n";

    for(const Navigation::TriangleNode& graphTriangle : NavMesh)
    {
        file << graphTriangle.GetIndex() << " ";
        file << graphTriangle.IsBlocked() << "\n";
    }

    file.close();
}

bool SNavigation::IsValidPoint(const v2 &point)
{
    for(const Navigation::TriangleNode& graphTriangle : NavMesh)
    {
        if (Util::IsPointInsideTriangle(graphTriangle.GetTriangle(), point))
        {
            return !graphTriangle.IsBlocked();
        }
    }
    return false;
}

}