#include "SNavigation.h"

#include <fstream>
#include <sstream>

#include "components/CFollow.h"
#include "components/CTransform.h"
#include "util/Util.h"
#include "components/CShapes.h"

namespace tX
{

void SNavigation::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    const auto followEntities = registry.view<CFollow, CTransform>();
    for(const entt::entity& followEntity : followEntities)
    {
        Process(followEntity, registry, deltaSeconds);
    }
}

void SNavigation::Process(entt::entity e, entt::registry& r, float deltaSeconds)
{
    CFollow& followComponent = r.get<CFollow>(e);
    EFollowState& bFollowing = followComponent.FollowState;
    if(bFollowing != EFollowState::Dirty)
        return;
    std::vector<v2>& stringPath = followComponent.StringPath;
    stringPath.clear();
    v2& goal = followComponent.Goal;

    v2& targetPos = followComponent.TargetPos;
    unsigned int& followIndex = followComponent.Index;
    v2 startPoint = {r.get<CTransform>(e).Position.x, r.get<CTransform>(e).Position.z};

    std::vector<Edge2D> portals;
    std::vector<Navigation::TriangleNode*> path;
    Navigation::AStar(startPoint, goal, path, portals, NavMesh);
    stringPath = Navigation::StringPull(portals, startPoint, goal);

    targetPos = stringPath[followIndex];
    followIndex = 1;
    bFollowing = EFollowState::Following;
}

void SNavigation::LoadNavMesh(entt::registry& r)
{
    std::ifstream file;
    file.open("../assets/navmesh/test.navmesh");
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

//        for(const Navigation::TriangleNode& graphTriangle : NavMesh)
//        {
//            auto e = r.create();
//            CTransform transform;
//            transform.Position = v3{graphTriangle.GetCircumCenter().x, 0.f, graphTriangle.GetCircumCenter().y};
//            r.emplace<CTransform>(e, transform);
//            if(graphTriangle.IsBlocked())
//            {
//                CTriangle2D triangle = graphTriangle.GetTriangle();
//                triangle.Color = FColor(0xFF0000FF);
//                r.emplace<CTriangle2D>(e, triangle);
//            }
//        }
    }
}

void SNavigation::SaveNavMesh()
{
    std::ofstream file;
    file.open("../assets/navmesh/test.navmesh");
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

void SNavigation::ClearNavMesh()
{
    NavMesh.clear();
    Points.clear();
}

void SNavigation::AddPoint(const v2 &point)
{
    Points.push_back(point);
}

void SNavigation::GenerateNavMesh()
{
    NavMesh = Navigation::BowyerWatson(Points);
    for(const Navigation::TriangleNode& graphTriangle : NavMesh)
    {
        entt::entity e = CreateEntity();
        CLineStrip lineStrip{};
        lineStrip.Color = FColor(0xFFFFFFFF);
        const CTriangle2D& triangle = graphTriangle.GetTriangle();
        lineStrip.Points.emplace_back(triangle.Vertices.A.x, 0.f, triangle.Vertices.A.y);
        lineStrip.Points.emplace_back(triangle.Vertices.B.x, 0.f, triangle.Vertices.B.y);
        lineStrip.Points.emplace_back(triangle.Vertices.C.x, 0.f, triangle.Vertices.C.y);
        AddComponent(e, lineStrip);
    }
}

void SNavigation::ToggleTriangle(const v2& point)
{
    for(Navigation::TriangleNode& graphTriangle : NavMesh)
    {
        if (Util::IsPointInsideTriangle(graphTriangle.GetTriangle(), point))
        {
            entt::entity e = CreateEntity();
            if(graphTriangle.IsBlocked())
            {
                graphTriangle.SetBlocked(false);
                CTriangle triangle;
                triangle.V3 = {graphTriangle.GetTriangle().Vertices.A.x, 0.f, graphTriangle.GetTriangle().Vertices.A.y};
                triangle.V2 = {graphTriangle.GetTriangle().Vertices.B.x, 0.f, graphTriangle.GetTriangle().Vertices.B.y};
                triangle.V1 = {graphTriangle.GetTriangle().Vertices.C.x, 0.f, graphTriangle.GetTriangle().Vertices.C.y};
                triangle.Color = FColor(0xFFFFFFFF);
                AddComponent(e, triangle);
            }
            else
            {
                graphTriangle.SetBlocked(true);
                CTriangle triangle;
                triangle.V3 = {graphTriangle.GetTriangle().Vertices.A.x, 0.f, graphTriangle.GetTriangle().Vertices.A.y};
                triangle.V2 = {graphTriangle.GetTriangle().Vertices.B.x, 0.f, graphTriangle.GetTriangle().Vertices.B.y};
                triangle.V1 = {graphTriangle.GetTriangle().Vertices.C.x, 0.f, graphTriangle.GetTriangle().Vertices.C.y};
                triangle.Color = FColor(0xFF0000FF);
                AddComponent(e, triangle);
            }
        }
    }
}

}