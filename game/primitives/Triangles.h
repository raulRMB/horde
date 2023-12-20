#ifndef TRIANGLES_H
#define TRIANGLES_H

#include "util/defines.h"
#include "Color.h"

namespace tX
{

struct Edge2D
{
    v2 vertices[2]{};
    struct CTriangle2D* triangle{};
};

struct ABC
{
    v2 A = {};
    v2 B = {};
    v2 C = {};
    ABC() = delete;
};

struct CTriangle2D
{
    union
    {
        ABC Vertices;
        v2 Indecies[3]{};
    };

    FColor Color;
    Edge2D Edges[3]{};

    CTriangle2D();
    explicit CTriangle2D(const ABC& vertices)
    {
        Vertices = vertices;

        Edges[0].vertices[0] = Vertices.A;
        Edges[0].vertices[1] = Vertices.B;
        Edges[1].vertices[0] = Vertices.B;
        Edges[1].vertices[1] = Vertices.C;
        Edges[2].vertices[0] = Vertices.C;
        Edges[2].vertices[1] = Vertices.A;
    }
    explicit CTriangle2D(const v2& A, const v2& B, const v2& C)
    {
        Vertices.A = A;
        Vertices.B = B;
        Vertices.C = C;

        Edges[0].vertices[0] = A;
        Edges[0].vertices[1] = B;
        Edges[1].vertices[0] = B;
        Edges[1].vertices[1] = C;
        Edges[2].vertices[0] = C;
        Edges[2].vertices[1] = A;
    }

    bool operator ==(const CTriangle2D& other) const
    {
        return Vertices.A == other.Vertices.A && Vertices.B == other.Vertices.B && Vertices.C == other.Vertices.C;
    }

    bool operator !=(const CTriangle2D& other) const
    {
        return !(*this == other);
    }
};

inline CTriangle2D::CTriangle2D()
{
    Vertices.A = {};
    Vertices.B = {};
    Vertices.C = {};

    Edges[0].vertices[0] = Vertices.A;
    Edges[0].vertices[1] = Vertices.B;
    Edges[1].vertices[0] = Vertices.B;
    Edges[1].vertices[1] = Vertices.C;
    Edges[2].vertices[0] = Vertices.C;
    Edges[2].vertices[1] = Vertices.A;
}

struct Triangle3D
{
    v3 A;
    v3 B;
    v3 C;
};

}

#endif //TRIANGLES_H
