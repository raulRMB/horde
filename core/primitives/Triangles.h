#ifndef TRIANGLES_H
#define TRIANGLES_H

#include <raylib.h>
#include "../util/raymathEx.h"

struct Edge2D
{
    Vector2 vertices[2]{};
    struct Triangle2D* triangle{};
};

struct ABC
{
    Vector2 A = {};
    Vector2 B = {};
    Vector2 C = {};
    ABC() = delete;
};

struct Triangle2D
{
    union
    {
        ABC Vertices;
        Vector2 Indecies[3]{};
    };

    Edge2D Edges[3]{};

    Triangle2D();
    explicit Triangle2D(const ABC& vertices)
    {
        Vertices = vertices;
    }
    explicit Triangle2D(const Vector2& A, const Vector2& B, const Vector2& C)
    {
        Vertices.A = A;
        Vertices.B = B;
        Vertices.C = C;
    }

    bool operator ==(const Triangle2D& other) const
    {
        return Vertices.A == other.Vertices.A && Vertices.B == other.Vertices.B && Vertices.C == other.Vertices.C;
    }

    bool operator !=(const Triangle2D& other) const
    {
        return !(*this == other);
    }
};

struct Triangle3D
{
    Vector3 A;
    Vector3 B;
    Vector3 C;
};

#endif //TRIANGLES_H
