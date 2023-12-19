#ifndef MESH_H
#define MESH_H

#include "util/defines.h"

namespace tX
{

typedef struct FMesh
{
    i32 vertexCount;        // Number of vertices stored in arrays
    i32 triangleCount;      // Number of triangles stored (indexed or not)

    // Vertex attributes data
    f32* vertices;        // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    f32* texcoords;       // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    f32* texcoords2;      // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
    f32* normals;         // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    f32* tangents;        // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
    u8* colors;      // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    u16* indices;    // Vertex indices (in case vertex data comes indexed)

    // Animation vertex data
    f32 *animVertices;    // Animated vertex positions (after bones transformations)
    f32 *animNormals;     // Animated normals (after bones transformations)
    u8 *boneIds; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning)
    f32 *boneWeights;     // Vertex bone weight, up to 4 bones influence by vertex (skinning)

    // OpenGL identifiers
    u32 vaoId;     // OpenGL Vertex Array Object id
    u32 *vboId;    // OpenGL Vertex Buffer Objects id (default vertex data)
} FMesh;

}

#endif //MESH_H
