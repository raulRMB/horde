#ifndef X_NAVIGATION_H
#define X_NAVIGATION_H

#include "glm/gtx/norm.hpp"
#include <vector>
#include "primitives/Triangles.h"

namespace tX::Navigation
{

bool ContainsEdge(const Triangle2D& triangle, const Edge2D& edge);
v3 Intersect(v3 planeP, v3 planeN, v3 rayP, v3 rayD);

class TriangleNode
{
    Triangle2D Triangle;
    unsigned int Index;
    std::vector<TriangleNode*> neighbors;
    float gCost = 0.f;
    float hCost = 0.f;
    v2 circumcenter;
    float circumradius;
    v2 incenter;
    TriangleNode* parent = nullptr;
    bool bBlocked = false;

public:

    TriangleNode(const Triangle2D& triangle, unsigned int index = 0);
    ~TriangleNode() = default;

    void AddNeighbor(TriangleNode* neighbor);
    void RemoveNeighbor(TriangleNode* neighbor);
    [[nodiscard]] inline const std::vector<TriangleNode*>& GetNeighbors() const { return neighbors; }
    [[nodiscard]] inline const Triangle2D& GetTriangle() const { return Triangle; }
    [[nodiscard]] inline const unsigned int GetIndex() const { return Index; }

    [[nodiscard]] inline const float GetFCost() const { return gCost + hCost; }
    [[nodiscard]] inline const float GetGCost() const { return gCost; }
    [[nodiscard]] inline const float GetHCost() const { return hCost; }

    [[nodiscard]] inline const v2& GetCircumCenter() const { return circumcenter; }
    [[nodiscard]] inline const v2& GetInCenter() const { return incenter; }
    [[nodiscard]] inline const float GetRadius() const { return circumradius; }

    [[nodiscard]] inline TriangleNode* GetParent() const { return parent; }

    [[nodiscard]] inline bool IsBlocked() const { return bBlocked; }
    inline void SetBlocked(bool blocked) { bBlocked = blocked; }

    inline void SetGCost(float cost) { gCost = cost; }
    inline void SetHCost(float cost) { hCost = cost; }

    void SetParent(TriangleNode *pNode) { parent = pNode; }

    bool operator==(const TriangleNode& other) const;

    bool operator!=(const TriangleNode& other) const;
    void SetIndex(unsigned int i);
};

void FindIncenter(const Triangle2D& triangle, v2& incenter);
void FindCircumcircle(const Triangle2D& triangle, glm::vec2& circumcenter, float& circumradius);
std::vector<TriangleNode> BowyerWatson(std::vector<v2>& points);
bool PointInTriangle(const v2& p, const Triangle2D& t);
void AStar(const v2 &start, const v2 &end, std::vector<TriangleNode*> &path, std::vector<Edge2D>& portals, std::vector<v2>& points);
void AStar(const v2 &start, const v2 &end, std::vector<TriangleNode*> &path, std::vector<Edge2D>& portals, std::vector<TriangleNode>& triangles);
float TriangleArea2(const v2& A, const v2& B, const v2& C);
std::vector<v2> StringPull(std::vector<Edge2D>& portals, const v2& start, const v2& end);
const Edge2D* GetSharedEdge(const Triangle2D& t1, const Triangle2D& t2);

bool IsOnRight(const v2& O, const v2& A, const v2& B);

inline void vcpy(float* v1, const float* v2)
{
    v1[0] = v2[0];
    v1[1] = v2[1];
}

inline float vdistsqr(const float* v1, const float* v2)
{
    const float dx = v2[0] - v1[0];
    const float dy = v2[1] - v1[1];
    return dx*dx + dy*dy;
}

inline float triarea2(const float* a, const float* b, const float* c)
{
    const float ax = b[0] - a[0];
    const float ay = b[1] - a[1];
    const float bx = c[0] - a[0];
    const float by = c[1] - a[1];
    return bx*ay - ax*by;
}

inline bool vequal(const float* a, const float* b)
{
    static const float eq = 0.001f*0.001f;
    return vdistsqr(a, b) < eq;
}

inline int stringPull(const float* portals, int nportals,
               float* pts, const int maxPts)
{
    // Find straight path.
    int npts = 0;
    // Init scan state
    float portalApex[2], portalLeft[2], portalRight[2];
    int apexIndex = 0, leftIndex = 0, rightIndex = 0;
    vcpy(portalApex, &portals[0]);
    vcpy(portalLeft, &portals[0]);
    vcpy(portalRight, &portals[2]);

    // Add start point.
    vcpy(&pts[npts*2], portalApex);
    npts++;

    for (int i = 1; i < nportals && npts < maxPts; ++i)
    {
        const float* left = &portals[i*4+0];
        const float* right = &portals[i*4+2];

        // Update right vertex.
        if (triarea2(portalApex, portalRight, right) <= 0.0f)
        {
            if (vequal(portalApex, portalRight) || triarea2(portalApex, portalLeft, right) > 0.0f)
            {
                // Tighten the funnel.
                vcpy(portalRight, right);
                rightIndex = i;
            }
            else
            {
                // Right over left, insert left to path and restart scan from portal left point.
                vcpy(&pts[npts*2], portalLeft);
                npts++;
                // Make current left the new apex.
                vcpy(portalApex, portalLeft);
                apexIndex = leftIndex;
                // Reset portal
                vcpy(portalLeft, portalApex);
                vcpy(portalRight, portalApex);
                leftIndex = apexIndex;
                rightIndex = apexIndex;
                // Restart scan
                i = apexIndex;
                continue;
            }
        }

        // Update left vertex.
        if (triarea2(portalApex, portalLeft, left) >= 0.0f)
        {
            if (vequal(portalApex, portalLeft) || triarea2(portalApex, portalRight, left) < 0.0f)
            {
                // Tighten the funnel.
                vcpy(portalLeft, left);
                leftIndex = i;
            }
            else
            {
                // Left over right, insert right to path and restart scan from portal right point.
                vcpy(&pts[npts*2], portalRight);
                npts++;
                // Make current right the new apex.
                vcpy(portalApex, portalRight);
                apexIndex = rightIndex;
                // Reset portal
                vcpy(portalLeft, portalApex);
                vcpy(portalRight, portalApex);
                leftIndex = apexIndex;
                rightIndex = apexIndex;
                // Restart scan
                i = apexIndex;
                continue;
            }
        }
    }
    // Append last point to path.
    if (npts < maxPts)
    {
        vcpy(&pts[npts*2], &portals[(nportals-1)*4+0]);
        npts++;
    }

    return npts;
}

typedef TriangleNode TriangleNode;

}
#endif //X_NAVIGATION_H
