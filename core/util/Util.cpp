#include "Util.h"
#include "components/Physics.h"

namespace raylib
{
#include <raylib.h>
}

#include "components/Attribute.h"
#include "components/RayCollision.h"
#include "components/Physics.h"
#include "components/Transform.h"
#include "primitives/Polygon.h"
#include "primitives/Triangles.h"
#include <chrono>

namespace tZ
{

v3 Util::RandVec3(int min, int max) {
    return v3 {
            static_cast<float>(raylib::GetRandomValue(min, max)),
            static_cast<float>(raylib::GetRandomValue(min, max)),
            static_cast<float>(raylib::GetRandomValue(min, max))
    };
}

long Util::GenerateTimestamp() {
    auto currentTime = std::chrono::system_clock::now();
    long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime.time_since_epoch()
    ).count();
    return timestamp;
}

tZ::FAttribute* Util::GetAttribute(tZ::CAttributes& ac, const std::string& attr)
{
    auto it = std::find_if(ac.attributes.begin(), ac.attributes.end(),
                                                          [&attr](const FAttribute& a)
                                                          {
                                                              return a.id == attr;
                                                          });
    if(it != ac.attributes.end())
    {
        return &(*it);
    }
    raylib::TraceLog(raylib::LOG_ERROR, "Attribute %s not found", attr.c_str());
    return nullptr;
}

FRayCollision Util::GetMouseCollision()
{
    raylib::Ray ray = raylib::GetMouseRay(raylib::GetMousePosition(), ToRaylibCamera(Game::Instance().GetActiveCamera()));
    raylib::Vector3 TopLeft = {-1000.0f, 0.0f, -1000.0f};
    raylib::Vector3 TopRight = {1000.0f, 0.0f, -1000.0f};
    raylib::Vector3 BottomLeft = {-1000.0f, 0.0f, 1000.0f};
    raylib::Vector3 BottomRight = {1000.0f, 0.0f, 1000.0f};

    raylib::RayCollision raylibCol = raylib::GetRayCollisionQuad(ray, TopRight, TopLeft, BottomLeft, BottomRight);
    FRayCollision collision{};
    collision.point = v3(raylibCol.point.x, raylibCol.point.y, raylibCol.point.z);
    collision.normal = v3(raylibCol.normal.x, raylibCol.normal.y, raylibCol.normal.z);
    collision.distance = raylibCol.distance;
    collision.hit = raylibCol.hit;
    return collision;
}

bool Util::Check2DCollision(CPhysics2D& x, CTransform& xT, CPhysics2D& y, CTransform& yT) {
    raylib::Vector2 xVec = raylib::Vector2{xT.Position.x, xT.Position.z};
    raylib::Vector2 yVec = raylib::Vector2{yT.Position.x, yT.Position.z};
    if(x.CollisionType == ECollision2DType::Circle && y.CollisionType == ECollision2DType::Circle)
    {
        return CheckCollisionCircles(xVec, x.CollisionRadius, yVec, y.CollisionRadius);
    }
    return false;
}

FColor Util::RandColor() {
    return FColor{
            static_cast<u8>(rand() % 255),
            static_cast<u8>(rand() % 255),
            static_cast<u8>(rand() % 255),
            255
    };
}

v3 Util::GetMouseWorldPosition()
{
    const raylib::Ray ray = raylib::GetMouseRay(raylib::GetMousePosition(), ToRaylibCamera(Game::Instance().GetActiveCamera()));
    constexpr raylib::Vector3 TopLeft = {-1000.0f, 0.0f, -1000.0f};
    constexpr raylib::Vector3 TopRight = {1000.0f, 0.0f, -1000.0f};
    constexpr raylib::Vector3 BottomLeft = {-1000.0f, 0.0f, 1000.0f};
    constexpr raylib::Vector3 BottomRight = {1000.0f, 0.0f, 1000.0f};
    const raylib::RayCollision Collision = raylib::GetRayCollisionQuad(ray, TopRight, TopLeft , BottomLeft, BottomRight);
    return v3(Collision.point.x, Collision.point.y, Collision.point.z);
}

v2 Util::GetMouseWorldPosition2D()
{
    const v3 mousePos3d = GetMouseWorldPosition();
    return {mousePos3d.x, mousePos3d.z};
}

v2 Util::GetClosetPointOusideTriangle(v2 p, const Triangle2D &t)
{
    v2 a = t.Vertices.A;
    v2 b = t.Vertices.B;
    v2 c = t.Vertices.C;

    v2 ab = b - a;
    v2 ac = c - a;
    v2 bc = c - b;

    v2 ap = p - a;
    v2 bp = p - b;
    v2 cp = p - c;

    v2 abapPerp = ab * (glm::dot(ab, ap) / glm::dot(ab, ab));
    v2 acapPerp = ac * (glm::dot(ac, ap) / glm::dot(ac, ac));
    v2 abpPerp = ab * (glm::dot(ab, bp) / glm::dot(ab, ab));
    v2 bcpPerp = bc * (glm::dot(bc, cp) / glm::dot(bc, bc));

    bool abpSameDirection = glm::dot(ab, bp) > 0;
    bool acpSameDirection = glm::dot(ac, cp) > 0;
    bool bcpSameDirection = glm::dot(bc, cp) > 0;

    v2 closestPoint = {};
    if(abpSameDirection && !acpSameDirection)
    {
        closestPoint = a + abapPerp;
    }
    else if(!abpSameDirection && acpSameDirection)
    {
        closestPoint = a + acapPerp;
    }
    else if(abpSameDirection && acpSameDirection)
    {
        if(abapPerp.length() < acapPerp.length())
        {
            closestPoint = a + abapPerp;
        }
        else
        {
            closestPoint = a + acapPerp;
        }
    }
    else
    {
        closestPoint = a;
    }

    if(bcpSameDirection)
    {
        closestPoint = closestPoint + bcpPerp;
    }

    return closestPoint;
}

v2 Util::ClosestPointOnLine(const v2& start, const v2& end, v2 point)
{
    v2 lineVector = end - start;
    v2 pointVector = point - start;
    float t = glm::dot(pointVector, lineVector) / glm::dot(lineVector, lineVector);
    t = std::max(0.0f, std::min(1.0f, t));
    return start + t * lineVector;
}

bool Util::IsPointInsideTriangle(const Triangle2D &triangle, const v2 &point)
{
    const v2& v0 = triangle.Vertices.A;
    const v2& v1 = triangle.Vertices.B;
    const v2& v2 = triangle.Vertices.C;

    double area_triangle = 0.5 * (-v1.y * v2.x + v0.y * (-v1.x + v2.x) + v0.x * (v1.y - v2.y) + v1.x * v2.y);
    double s = 1.0 / (2.0 * area_triangle) * (v0.y * v2.x - v0.x * v2.y + (v2.y - v0.y) * point.x + (v0.x - v2.x) * point.y);
    double t = 1.0 / (2.0 * area_triangle) * (v0.x * v1.y - v0.y * v1.x + (v0.y - v1.y) * point.x + (v1.x - v0.x) * point.y);

    return s > 0 && t > 0 && 1 - s - t > 0;
}

v2 Util::MinimumTranslationVector(const Polygon2D &polygon, const v2 &point)
{
    if (polygon.Vertices.size() < 3)
    {
        raylib::TraceLog(raylib::LOG_ERROR, "Error: Polygon must have at least 3 vertices.\n");
        return {0.0, 0.0};
    }

    v2 mtv = {0.0, 0.0};
    double minDistance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < polygon.Vertices.size(); ++i)
    {
        const v2& start = polygon.Vertices[i];
        const v2& end = polygon.Vertices[(i + 1) % polygon.Vertices.size()];

        v2 edgeVector = end - start;
        v2 pointToStart = start - point;

        float t = glm::dot(pointToStart, edgeVector) / glm::dot(edgeVector, edgeVector);
        t = std::max(0.0f, std::min(1.0f, abs(t)));

        v2 closestPointOnEdge = start + t * edgeVector;

        double dist = (point - closestPointOnEdge).length();

        if (dist < minDistance)
        {
            minDistance = dist;
            mtv = point - closestPointOnEdge;
        }
    }

    return mtv;
}

}
