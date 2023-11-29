#include "Util.h"
#include "components/Physics.h"

glm::mat4 Util::GLMMat4FromMatrix(const Matrix& matrix)
{
    glm::mat4 result{};
    result[0][0] = matrix.m0;
    result[0][1] = matrix.m1;
    result[0][2] = matrix.m2;
    result[0][3] = matrix.m3;
    result[1][0] = matrix.m4;
    result[1][1] = matrix.m5;
    result[1][2] = matrix.m6;
    result[1][3] = matrix.m7;
    result[2][0] = matrix.m8;
    result[2][1] = matrix.m9;
    result[2][2] = matrix.m10;
    result[2][3] = matrix.m11;
    result[3][0] = matrix.m12;
    result[3][1] = matrix.m13;
    result[3][2] = matrix.m14;
    result[3][3] = matrix.m15;

    return result;
}

Vector3 Util::RandVec3(int min, int max) {
    return Vector3 {
            (float)GetRandomValue(min, max),
            (float)GetRandomValue(min, max),
            (float)GetRandomValue(min, max)
    };
}

FAttribute* Util::GetAttribute(CAttributes& ac, const std::string& attr)
{
    auto it = std::find_if(ac.attributes.begin(), ac.attributes.end(), [&attr](const FAttribute &a)
    {
        return a.id == attr;
    });
    if(it != ac.attributes.end())
    {
        return &(*it);
    }
    TraceLog(LOG_ERROR, "Attribute %s not found", attr.c_str());
    return nullptr;
}

RayCollision Util::GetMouseCollision() {
    Ray ray = GetMouseRay(GetMousePosition(), Game::Instance().GetActiveCamera());
    Vector3 TopLeft = {-1000.0f, 0.0f, -1000.0f};
    Vector3 TopRight = {1000.0f, 0.0f, -1000.0f};
    Vector3 BottomLeft = {-1000.0f, 0.0f, 1000.0f};
    Vector3 BottomRight = {1000.0f, 0.0f, 1000.0f};
    return GetRayCollisionQuad(ray, TopRight, TopLeft, BottomLeft, BottomRight);
}

bool Util::Check2DCollision(CPhysics2D& x, Transform& xT, CPhysics2D& y, Transform& yT) {
    Vector2 xVec = Vector2{xT.translation.x, xT.translation.z};
    Vector2 yVec = Vector2{yT.translation.x, yT.translation.z};
    if(x.CollisionType == ECollision2DType::Circle && y.CollisionType == ECollision2DType::Circle)
    {
        return CheckCollisionCircles(xVec, x.CollisionRadius, yVec, y.CollisionRadius);
    }
    return false;
}

Color Util::RandColor() {
    return Color {
            (unsigned char) GetRandomValue(1, 255),
            (unsigned char) GetRandomValue(1, 255),
            (unsigned char) GetRandomValue(1, 255),
            255
    };
}

Vector3 Util::GetMouseWorldPosition()
{
    const Ray ray = GetMouseRay(GetMousePosition(), Game::Instance().GetActiveCamera());
    constexpr Vector3 TopLeft = {-1000.0f, 0.0f, -1000.0f};
    constexpr Vector3 TopRight = {1000.0f, 0.0f, -1000.0f};
    constexpr Vector3 BottomLeft = {-1000.0f, 0.0f, 1000.0f};
    constexpr Vector3 BottomRight = {1000.0f, 0.0f, 1000.0f};
    const RayCollision Collision = GetRayCollisionQuad(ray, TopRight, TopLeft , BottomLeft, BottomRight);
    return Collision.point;
}

Vector2 Util::GetMouseWorldPosition2D()
{
    const Vector3 mousePos3d = GetMouseWorldPosition();
    return {mousePos3d.x, mousePos3d.z};
}

Vector2 Util::GetClosetPointOusideTriangle(Vector2 p, const Triangle2D &t) {
    Vector2 a = t.Vertices.A;
    Vector2 b = t.Vertices.B;
    Vector2 c = t.Vertices.C;

    Vector2 ab = Vector2Subtract(b, a);
    Vector2 ac = Vector2Subtract(c, a);
    Vector2 bc = Vector2Subtract(c, b);

    Vector2 ap = Vector2Subtract(p, a);
    Vector2 bp = Vector2Subtract(p, b);
    Vector2 cp = Vector2Subtract(p, c);

    Vector2 abapPerp = Vector2Scale(ab, Vector2DotProduct(ab, ap) / Vector2DotProduct(ab, ab));
    Vector2 acapPerp = Vector2Scale(ac, Vector2DotProduct(ac, ap) / Vector2DotProduct(ac, ac));
    Vector2 abpPerp = Vector2Scale(ab, Vector2DotProduct(ab, bp) / Vector2DotProduct(ab, ab));
    Vector2 bcpPerp = Vector2Scale(bc, Vector2DotProduct(bc, cp) / Vector2DotProduct(bc, bc));

    bool abpSameDirection = Vector2DotProduct(ab, bp) > 0;
    bool acpSameDirection = Vector2DotProduct(ac, cp) > 0;
    bool bcpSameDirection = Vector2DotProduct(bc, cp) > 0;

    Vector2 closestPoint = {};
    if(abpSameDirection && !acpSameDirection)
    {
        closestPoint = Vector2Add(a, abapPerp);
    }
    else if(!abpSameDirection && acpSameDirection)
    {
        closestPoint = Vector2Add(a, acapPerp);
    }
    else if(abpSameDirection && acpSameDirection)
    {
        if(Vector2Length(abapPerp) < Vector2Length(acapPerp))
        {
            closestPoint = Vector2Add(a, abapPerp);
        }
        else
        {
            closestPoint = Vector2Add(a, acapPerp);
        }
    }
    else
    {
        closestPoint = a;
    }

    if(bcpSameDirection)
    {
        closestPoint = Vector2Add(closestPoint, bcpPerp);
    }

    return closestPoint;
}

Vector2 Util::ClosestPointOnLine(const Vector2& start, const Vector2& end, Vector2 point)
{
    Vector2 lineVector = end - start;
    Vector2 pointVector = point - start;
    double t = Vector2DotProduct(pointVector, lineVector) / Vector2DotProduct(lineVector, lineVector);
    t = std::max(0.0, std::min(1.0, t));
    return start + t * lineVector;
}

bool Util::IsPointInsideTriangle(const Triangle2D &triangle, const Vector2 &point)
{
    const Vector2& v0 = triangle.Vertices.A;
    const Vector2& v1 = triangle.Vertices.B;
    const Vector2& v2 = triangle.Vertices.C;

    double area_triangle = 0.5 * (-v1.y * v2.x + v0.y * (-v1.x + v2.x) + v0.x * (v1.y - v2.y) + v1.x * v2.y);
    double s = 1.0 / (2.0 * area_triangle) * (v0.y * v2.x - v0.x * v2.y + (v2.y - v0.y) * point.x + (v0.x - v2.x) * point.y);
    double t = 1.0 / (2.0 * area_triangle) * (v0.x * v1.y - v0.y * v1.x + (v0.y - v1.y) * point.x + (v1.x - v0.x) * point.y);

    return s > 0 && t > 0 && 1 - s - t > 0;
}

Vector2 Util::MinimumTranslationVector(const Polygon2D &polygon, const Vector2 &point)
{
    if (polygon.Vertices.size() < 3)
    {
        TraceLog(LOG_ERROR, "Error: Polygon must have at least 3 vertices.\n");
        return {0.0, 0.0};
    }

    Vector2 mtv = {0.0, 0.0};
    double minDistance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < polygon.Vertices.size(); ++i)
    {
        const Vector2& start = polygon.Vertices[i];
        const Vector2& end = polygon.Vertices[(i + 1) % polygon.Vertices.size()];

        Vector2 edgeVector = end - start;
        Vector2 pointToStart = start - point;

        double t = Vector2DotProduct(pointToStart, edgeVector) / Vector2DotProduct(edgeVector, edgeVector);
        t = std::max(0.0, std::min(1.0, abs(t)));

        Vector2 closestPointOnEdge = start + t * edgeVector;

        double dist = Vector2Length(point - closestPointOnEdge);

        if (dist < minDistance)
        {
            minDistance = dist;
            mtv = point - closestPointOnEdge;
        }
    }

    return mtv;
}
