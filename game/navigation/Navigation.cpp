#include "Navigation.h"
#include <vector>
#include <algorithm>
#include <set>

namespace tX::Navigation
{

bool ContainsEdge(const CTriangle2D& triangle, const Edge2D& edge)
{
    int sharedVerts = 0;
    for (const v2 &vertex : triangle.Indecies)
    {
        if (vertex == edge.vertices[0] || vertex == edge.vertices[1])
        {
            sharedVerts++;
        }
    }
    return sharedVerts == 2;
}

v3 Intersect(v3 planeP, v3 planeN, v3 rayP, v3 rayD)
{
    float d = glm::dot(planeP, -planeN);
    float t = -(d + glm::dot(rayP, planeN)) / glm::dot(rayD, planeN);
    return rayP + rayD * t;
}

void FindIncenter(const CTriangle2D& triangle, v2& incenter)
{
    float a = glm::distance(triangle.Vertices.B, triangle.Vertices.C);
    float b = glm::distance(triangle.Vertices.A, triangle.Vertices.C);
    float c = glm::distance(triangle.Vertices.A, triangle.Vertices.B);

    incenter = (a * triangle.Vertices.A + b * triangle.Vertices.B + c * triangle.Vertices.C) / (a + b + c);
}

void FindCircumcircle(const CTriangle2D &triangle, v2 &circumcenter, float &circumradius)
{
    v2 A = triangle.Vertices.A;
    v2 B = triangle.Vertices.B;
    v2 C = triangle.Vertices.C;

    float D = 2.0f * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
    if (std::abs(D) < glm::epsilon<float>()) {
        circumcenter = {};
        circumradius = 0.0f;
        return;
    }

    circumcenter.x = (1.f/D) * ((A.x * A.x + A.y * A.y) * (B.y - C.y) +
                                (B.x * B.x + B.y * B.y) * (C.y - A.y) +
                                (C.x * C.x + C.y * C.y) * (A.y - B.y));
    circumcenter.y = (1.f/D) * ((A.x * A.x + A.y * A.y) * (C.x - B.x) +
                                (B.x * B.x + B.y * B.y) * (A.x - C.x) +
                                (C.x * C.x + C.y * C.y) * (B.x - A.x));

    circumradius = glm::distance(A, circumcenter);
}

std::vector<TriangleNode> BowyerWatson(std::vector<v2>& points)
{
    TriangleNode supraTriangle = TriangleNode(CTriangle2D({-1000.0f, -1000.0f}, {1000.0f, -1000.0f}, {0.0f, 1000.0f}));

    std::vector<TriangleNode> graphTriangles = {supraTriangle};
    for (v2& point : points)
    {
        std::vector<TriangleNode> badTriangles = {};
        for (TriangleNode& triangle : graphTriangles)
        {
            v2 circumCenter;
            float circumRadius;
            FindCircumcircle(triangle.GetTriangle(), circumCenter, circumRadius);
            if(glm::distance(point, circumCenter) < circumRadius)
            {
                badTriangles.push_back(triangle);
            }
        }

        std::vector<Edge2D> polygon = {};
        for (int i = 0; i < badTriangles.size(); i++)
        {
            const CTriangle2D& triangle = badTriangles[i].GetTriangle();
            for (const Edge2D& edge : triangle.Edges)
            {
                bool rejectEdge = false;
                for (int t = 0; t < badTriangles.size(); t++)
                {
                    if (t != i && ContainsEdge(badTriangles[t].GetTriangle(), edge))
                    {
                        rejectEdge = true;
                        break;
                    }
                }
                if (!rejectEdge){
                    polygon.push_back(edge);
                }
            }
        }

        graphTriangles.erase(std::remove_if(graphTriangles.begin(), graphTriangles.end(), [&](const Navigation::TriangleNode& t) {
            return std::find(badTriangles.begin(), badTriangles.end(), t) != badTriangles.end();
        }), graphTriangles.end());

        for(const Edge2D& edge : polygon)
        {
            Navigation::TriangleNode graphTriangle = Navigation::TriangleNode(CTriangle2D(edge.vertices[0], edge.vertices[1], point));
            graphTriangles.emplace_back(graphTriangle);
        }
    }

    graphTriangles.erase(std::remove_if(graphTriangles.begin(), graphTriangles.end(), [&](const Navigation::TriangleNode& t)
    {
        return t.GetTriangle().Vertices.A == supraTriangle.GetTriangle().Vertices.A ||
               t.GetTriangle().Vertices.A == supraTriangle.GetTriangle().Vertices.B ||
               t.GetTriangle().Vertices.A == supraTriangle.GetTriangle().Vertices.C ||
               t.GetTriangle().Vertices.B == supraTriangle.GetTriangle().Vertices.A ||
               t.GetTriangle().Vertices.B == supraTriangle.GetTriangle().Vertices.B ||
               t.GetTriangle().Vertices.B == supraTriangle.GetTriangle().Vertices.C ||
               t.GetTriangle().Vertices.C == supraTriangle.GetTriangle().Vertices.A ||
               t.GetTriangle().Vertices.C == supraTriangle.GetTriangle().Vertices.B ||
               t.GetTriangle().Vertices.C == supraTriangle.GetTriangle().Vertices.C;
    }), graphTriangles.end());

    unsigned int idx = 0;
    for(Navigation::TriangleNode& graphTriangle : graphTriangles)
    {
        graphTriangle.SetIndex(idx++);
        const CTriangle2D& t = graphTriangle.GetTriangle();
        for(Navigation::TriangleNode& neighbor : graphTriangles)
        {
            for(const Edge2D& edge : t.Edges)
            {
                if(ContainsEdge(neighbor.GetTriangle(), edge))
                {
                    if(graphTriangle != neighbor)
                    {
                        graphTriangle.AddNeighbor(&neighbor);
                    }
                }
            }
        }
    }
    return graphTriangles;
}

bool PointInTriangle(const v2 &p, const CTriangle2D &triangle)
{
    const v2* v = triangle.Indecies;

    float s = (v[0].x - v[2].x) * (p.y - v[2].y) - (v[0].y - v[2].y) * (p.x - v[2].x);
    float t = (v[1].x - v[0].x) * (p.y - v[0].y) - (v[1].y - v[0].y) * (p.x - v[0].x);

    if ((s < 0) != (t < 0) && s != 0 && t != 0)
        return false;

    float d = (v[2].x - v[1].x) * (p.y - v[1].y) - (v[2].y - v[1].y) * (p.x - v[1].x);
    return d == 0 || (d < 0) == (s + t <= 0);
}

std::vector<TriangleNode*> ReconstructPath(TriangleNode* end, TriangleNode* start, std::vector<Edge2D>& portals)
{
    std::vector<TriangleNode*> path;
    TriangleNode* current = end;
    while(current != start)
    {
        path.push_back(current);
        if(current->GetParent())
        {
            current = current->GetParent();
        }
    }
    path.push_back(start);

    std::reverse(path.begin(), path.end());

    for(int i = 0; i < path.size() - 1; i++)
    {
        if(const Edge2D* SharedEdge = Navigation::GetSharedEdge(path[i]->GetTriangle(), path[i + 1]->GetTriangle()); SharedEdge != nullptr)
        {
            if(IsOnRight(path[i]->GetCircumCenter(), path[i + 1]->GetCircumCenter(), SharedEdge->vertices[0]))
            {
                v2 directionZero = glm::normalize(path[i]->GetInCenter() - SharedEdge->vertices[0]);
                v2 directionOne = glm::normalize(path[i]->GetInCenter() - SharedEdge->vertices[1]);

                float Offset = 2.f;
                v2 offsetZero = directionZero * Offset;
                v2 offsetOne = directionOne * Offset;
                portals.push_back({SharedEdge->vertices[1] + offsetOne, SharedEdge->vertices[0] + offsetZero});
            }
            else
            {
                portals.push_back(*SharedEdge);
            }
        }
    }

    return path;
}

void AStar(const v2 &start, const v2 &end, std::vector<TriangleNode*> &path, std::vector<Edge2D>& portals, std::vector<v2>& points)
{
    TriangleNode* startTriangle = nullptr;
    TriangleNode* endTriangle = nullptr;
    std::set<TriangleNode*> open;
    std::set<TriangleNode*> closed;

    std::vector<TriangleNode> graphTriangles = BowyerWatson(points);

    for(TriangleNode& graphTriangle : graphTriangles)
    {
        if(PointInTriangle(start, graphTriangle.GetTriangle()))
        {
            startTriangle = &graphTriangle;
        }
        if(PointInTriangle(end, graphTriangle.GetTriangle()))
        {
            endTriangle = &graphTriangle;
        }
    }

    if(startTriangle == nullptr || endTriangle == nullptr)
    {
        return;
    }

    open.emplace(startTriangle);

    while(!open.empty())
    {
        TriangleNode *current = *open.begin();

        for (TriangleNode *node : open)
        {
            if (node->GetFCost() < current->GetFCost() || node->GetFCost() == current->GetFCost() && node->GetHCost() < current->GetHCost())
            {
                current = node;
            }
        }

        open.erase(open.find(current));
        closed.emplace(current);

        if(current == endTriangle)
        {
            break;
        }

        for(Navigation::TriangleNode* neighbor : current->GetNeighbors())
        {
            if(closed.find(neighbor) != closed.end() || neighbor->IsBlocked())
            {
                continue;
            }

            float newMovementCostToNeighbor = current->GetGCost() + glm::distance(current->GetCircumCenter(),
                                                                                    neighbor->GetCircumCenter());
            if(newMovementCostToNeighbor < neighbor->GetGCost() || open.find(neighbor) == open.end())
            {
                neighbor->SetGCost(newMovementCostToNeighbor);
                neighbor->SetHCost(glm::distance(neighbor->GetCircumCenter(), endTriangle->GetCircumCenter()));
                neighbor->SetParent(current);
                open.emplace(neighbor);
            }
        }
    }

    if(!portals.empty()) {
        path = ReconstructPath(endTriangle, startTriangle, portals);
    }
}

void AStar(const v2 &start, const v2 &end, std::vector<TriangleNode*> &path, std::vector<Edge2D>& portals, std::vector<TriangleNode>& graphTriangles)
{
    TriangleNode* startTriangle = nullptr;
    TriangleNode* endTriangle = nullptr;
    std::set<TriangleNode*> open;
    std::set<TriangleNode*> closed;

    for(TriangleNode& graphTriangle : graphTriangles)
    {
        if(PointInTriangle(start, graphTriangle.GetTriangle()))
        {
            startTriangle = &graphTriangle;
        }
        if(PointInTriangle(end, graphTriangle.GetTriangle()))
        {
            endTriangle = &graphTriangle;
        }
    }

    if(startTriangle == nullptr || endTriangle == nullptr)
    {
        return;
    }

    open.emplace(startTriangle);

    while(!open.empty())
    {
        TriangleNode *current = *open.begin();

        for (TriangleNode *node : open)
        {
            if (node->GetFCost() < current->GetFCost() || node->GetFCost() == current->GetFCost() && node->GetHCost() < current->GetHCost())
            {
                current = node;
            }
        }

        open.erase(open.find(current));
        closed.emplace(current);

        if(current == endTriangle)
        {
            break;
        }

        for(TriangleNode* neighbor : current->GetNeighbors())
        {
            if(closed.find(neighbor) != closed.end() || neighbor->IsBlocked())
            {
                continue;
            }

            float newMovementCostToNeighbor = current->GetGCost() + glm::distance(current->GetCircumCenter(),
                                                                                    neighbor->GetCircumCenter());
            if(newMovementCostToNeighbor < neighbor->GetGCost() || open.find(neighbor) == open.end())
            {
                neighbor->SetGCost(newMovementCostToNeighbor);
                neighbor->SetHCost(glm::distance(neighbor->GetCircumCenter(), endTriangle->GetCircumCenter()));
                neighbor->SetParent(current);
                open.emplace(neighbor);
            }
        }
    }

    path = ReconstructPath(endTriangle, startTriangle, portals);
}

float TriangleArea2(const v2 &A, const v2 &B, const v2 &C)
{
    const float ax = B.x - A.x;
    const float ay = B.y - A.y;
    const float bx = C.x - A.x;
    const float by = C.y - A.y;
    return bx*ay - ax*by;
}

struct CFunnelPoint
{
    v2 pos;

    void operator = (const v2& pos)
    {
        this->pos = pos;
    }
};

std::vector<v2> StringPull(std::vector<Edge2D> &portals, const v2 &start, const v2 &end)
{
    std::vector<v2> path;


    v2 portalApex = start;
    v2 portalLeft{};
    v2 portalRight{};
    if(!portals.empty())
    {
        portalLeft = portals[0].vertices[0];
        portalRight = portals[0].vertices[1];
    }

    portals.push_back({end, end});
    int apexIndex = 0, leftIndex = 0, rightIndex = 0;

    CFunnelPoint funnelLeft{};
    funnelLeft = portalLeft;
    CFunnelPoint funnelRight{};
    funnelRight = portalRight;
    CFunnelPoint funnelApex{};
    funnelApex = portalApex;

    path.push_back(portalApex);

    for(int i = 1; i < portals.size(); ++i)
    {
        const v2& left = portals[i].vertices[0];
        const v2& right = portals[i].vertices[1];

        float area = Navigation::TriangleArea2(portalApex, portalRight, right);
        if(area <= 0.0f)
        {
            area = Navigation::TriangleArea2(portalApex, portalLeft, right);
            if(portalApex == portalRight ||
                    area > 0.0f)
            {
                portalRight = right;
                rightIndex = i;
            }
            else
            {
                path.push_back(portalLeft);
                apexIndex = leftIndex;
                portalApex = portalLeft;
                portalLeft = portalApex;
                portalRight = portalApex;
                leftIndex = apexIndex;
                rightIndex = apexIndex;
                i = apexIndex;
                continue;
            }
        }
        area = TriangleArea2(portalApex, portalLeft, left);
        if(area >= 0.0f)
        {
            area = TriangleArea2(portalApex, portalRight, left);
            if(portalApex == portalLeft ||
                    area < 0.0f)
            {
                portalLeft = left;
                leftIndex = i;
            }
            else
            {
                path.push_back(portalRight);
                apexIndex = rightIndex;
                portalApex = portalRight;
                portalLeft = portalApex;
                portalRight = portalApex;
                leftIndex = apexIndex;
                rightIndex = apexIndex;
                i = apexIndex;
                continue;
            }
        }
    }

    if(path[path.size() - 1] != end)
    {
        path.push_back(end);
    }

    return path;
}


const Edge2D* GetSharedEdge(const CTriangle2D &t1, const CTriangle2D &t2)
{
    for(const Edge2D& edge : t1.Edges)
    {
        if(ContainsEdge(t2, edge))
        {
            return &edge;
        }
    }
    return nullptr;
}

bool IsOnRight(const v2 &O, const v2 &A, const v2 &B)
{
    v2 a = glm::normalize(A - O);
    v2 b = glm::normalize(B - O);

    return a.x * -b.y + a.y * b.x > 0;
}

void TriangleNode::AddNeighbor(TriangleNode *neighbor)
{
    if(std::find(Neighbors.begin(), Neighbors.end(), neighbor) != Neighbors.end() && *Neighbors.end() == neighbor)
        return;

    if(std::find(neighbor->Neighbors.begin(), neighbor->Neighbors.end(), this) != neighbor->Neighbors.end() && *neighbor->Neighbors.end() == this)
        return;

    Neighbors.push_back(neighbor);
}

void TriangleNode::RemoveNeighbor(TriangleNode *neighbor)
{
    Neighbors.erase(std::remove(Neighbors.begin(), Neighbors.end(), neighbor), Neighbors.end());
}

bool TriangleNode::operator==(const TriangleNode &other) const
{
    return GetTriangle() == other.GetTriangle();
}

bool TriangleNode::operator!=(const TriangleNode &other) const
{
    return !(*this == other);
}

void TriangleNode::SetIndex(unsigned int i)
{
    Index = i;
}

TriangleNode::TriangleNode(const CTriangle2D &triangle, unsigned int index) :
        Triangle(triangle), Index(index),
        Circumcenter({}) , Circumradius(0.f),
        Incenter({}), Parent(nullptr), bBlocked(false)
{
    FindCircumcircle(triangle, Circumcenter, Circumradius);
    FindIncenter(triangle, Incenter);
}
}
