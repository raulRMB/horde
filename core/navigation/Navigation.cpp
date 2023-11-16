#include "Navigation.h"
#include <vector>
#include <algorithm>
#include "../util/raymathEx.h"
#include <set>

namespace Navigation
{
bool ContainsEdge(const Triangle2D& triangle, const Edge2D& edge)
{
    int sharedVerts = 0;
    for (const Vector2 &vertex : triangle.Indecies)
    {
        if (vertex == edge.vertices[0] || vertex == edge.vertices[1])
        {
            sharedVerts++;
        }
    }
    return sharedVerts == 2;
}

    void FindIncenter(const Triangle2D& triangle, Vector2& incenter)
{
    float a = Vector2Distance(triangle.Vertices.B, triangle.Vertices.C);
    float b = Vector2Distance(triangle.Vertices.A, triangle.Vertices.C);
    float c = Vector2Distance(triangle.Vertices.A, triangle.Vertices.B);

    incenter = (a * triangle.Vertices.A + b * triangle.Vertices.B + c * triangle.Vertices.C) / (a + b + c);
}

    void FindCircumcircle(const Triangle2D &triangle, Vector2 &circumcenter, float &circumradius)
{
    Vector2 A = triangle.Vertices.A;
    Vector2 B = triangle.Vertices.B;
    Vector2 C = triangle.Vertices.C;

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

    circumradius = Vector2Distance(A, circumcenter);
}

std::vector<TriangleNode> BowyerWatson(std::vector<Vector2>& points)
{
    TriangleNode supraTriangle = TriangleNode(Triangle2D({-1000.0f, -1000.0f}, {1000.0f, -1000.0f}, {0.0f, 1000.0f}));

    std::vector<TriangleNode> graphTriangles = {supraTriangle};
    for (Vector2& point : points)
    {
        std::vector<TriangleNode> badTriangles = {};
        for (TriangleNode& triangle : graphTriangles)
        {
            Vector2 circumCenter;
            float circumRadius;
            FindCircumcircle(triangle.GetTriangle(), circumCenter, circumRadius);
            if(Vector2Distance(point, circumCenter) < circumRadius)
            {
                badTriangles.push_back(triangle);
            }
        }

        std::vector<Edge2D> polygon = {};
        for (int i = 0; i < badTriangles.size(); i++)
        {
            const Triangle2D& triangle = badTriangles[i].GetTriangle();
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
            Navigation::TriangleNode graphTriangle = Navigation::TriangleNode(Triangle2D(edge.vertices[0], edge.vertices[1], point));
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
        const Triangle2D& t = graphTriangle.GetTriangle();
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

bool PointInTriangle(const Vector2 &p, const Triangle2D &triangle)
{
    const Vector2* v = triangle.Indecies;

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
        current = current->GetParent();
    }
    path.push_back(start);

    std::reverse(path.begin(), path.end());

    for(int i = 0; i < path.size() - 1; i++)
    {
        if(const Edge2D* SharedEdge = Navigation::GetSharedEdge(path[i]->GetTriangle(), path[i + 1]->GetTriangle()); SharedEdge != nullptr)
        {
            if(IsOnRight(path[i]->GetCenter(), path[i + 1]->GetCenter(), SharedEdge->vertices[0]))
            {
                portals.push_back({SharedEdge->vertices[1], SharedEdge->vertices[0]});
            }
            else
            {
                portals.push_back(*SharedEdge);
            }
        }
    }

    return path;
}

void AStar(const Vector2 &start, const Vector2 &end, std::vector<TriangleNode*> &path, std::vector<Edge2D>& portals, std::vector<Vector2>& points)
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

            float newMovementCostToNeighbor = current->GetGCost() + Vector2Distance(current->GetCenter(), neighbor->GetCenter());
            if(newMovementCostToNeighbor < neighbor->GetGCost() || open.find(neighbor) == open.end())
            {
                neighbor->SetGCost(newMovementCostToNeighbor);
                neighbor->SetHCost(Vector2Distance(neighbor->GetCenter(), endTriangle->GetCenter()));
                neighbor->SetParent(current);
                open.emplace(neighbor);
            }
        }
    }

    path = ReconstructPath(endTriangle, startTriangle, portals);
}

void AStar(const Vector2 &start, const Vector2 &end, std::vector<TriangleNode*> &path, std::vector<Edge2D>& portals, std::vector<TriangleNode>& graphTriangles)
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

            float newMovementCostToNeighbor = current->GetGCost() + Vector2Distance(current->GetCenter(), neighbor->GetCenter());
            if(newMovementCostToNeighbor < neighbor->GetGCost() || open.find(neighbor) == open.end())
            {
                neighbor->SetGCost(newMovementCostToNeighbor);
                neighbor->SetHCost(Vector2Distance(neighbor->GetCenter(), endTriangle->GetCenter()));
                neighbor->SetParent(current);
                open.emplace(neighbor);
            }
        }
    }

    path = ReconstructPath(endTriangle, startTriangle, portals);
}

float TriangleArea2(const Vector2 &A, const Vector2 &B, const Vector2 &C)
{
    const float ax = B.x - A.x;
    const float ay = B.y - A.y;
    const float bx = C.x - A.x;
    const float by = C.y - A.y;
    return bx*ay - ax*by;
}

struct CFunnelLeft
{
    Vector2 pos;

    void operator = (const Vector2& pos)
    {
        this->pos = pos;
    }
} funnelLeft;

struct CFunnelRight
{
    Vector2 pos;

    void operator = (const Vector2& pos)
    {
        this->pos = pos;
    }
} funnelRight;

struct CFunnelApex
{
    Vector2 pos;

    void operator = (const Vector2& pos)
    {
        this->pos = pos;
    }
} funnelApex;


std::vector<Vector2> StringPull(const std::vector<Edge2D> &portals, const Vector2 &start, const Vector2 &end)
{
    std::vector<Vector2> path;

    Vector2 portalApex = start;
    Vector2 portalLeft = portals[0].vertices[0];
    Vector2 portalRight = portals[0].vertices[1];

    const_cast<std::vector<Edge2D>&>(portals).push_back({end, end});
    int apexIndex = 0, leftIndex = 0, rightIndex = 0;

    funnelLeft = portalLeft;
    funnelRight = portalRight;
    funnelApex = portalApex;

    path.push_back(portalApex);

    for(int i = 1; i < portals.size(); ++i)
    {
        const Vector2& left = portals[i].vertices[0];
        const Vector2& right = portals[i].vertices[1];

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


const Edge2D* GetSharedEdge(const Triangle2D &t1, const Triangle2D &t2)
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

bool IsOnRight(const Vector2 &O, const Vector2 &A, const Vector2 &B)
{
    Vector2 a = Vector2Normalize(A - O);
    Vector2 b = Vector2Normalize(B - O);

    return a.x * -b.y + a.y * b.x > 0;
}

void TriangleNode::AddNeighbor(TriangleNode *neighbor)
{
    if(std::find(neighbors.begin(), neighbors.end(), neighbor) != neighbors.end() && *neighbors.end() == neighbor)
        return;

    if(std::find(neighbor->neighbors.begin(), neighbor->neighbors.end(), this) != neighbor->neighbors.end() && *neighbor->neighbors.end() == this)
        return;

    neighbors.push_back(neighbor);
}

void TriangleNode::RemoveNeighbor(TriangleNode *neighbor)
{
    neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), neighbor), neighbors.end());
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

TriangleNode::TriangleNode(const Triangle2D &triangle, unsigned int index) :
    triangle(triangle), Index(index),
    circumcenter({}) , circumradius(0.f),
    incenter({}), parent(nullptr), bBlocked(false)
{
    FindCircumcircle(triangle, circumcenter, circumradius);
    FindIncenter(triangle, incenter);
}
}