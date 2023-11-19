#ifndef RAYMATHEX_H
#define RAYMATHEX_H

#include <raymath.h>

#pragma region Vector2

inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2Add(v1, v2);
}

inline Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2Subtract(v1, v2);
}

inline Vector2 operator*(const Vector2& v1, const Vector2& v2)
{
    return Vector2Multiply(v1, v2);
}

inline Vector2 operator*(const float& f, const Vector2& v)
{
    return Vector2Scale(v, f);
}

inline Vector2 operator*(const Vector2& v, const float& f)
{
    return Vector2Scale(v, f);
}

inline Vector2 operator/(const Vector2& v1, const Vector2& v2)
{
    return Vector2Divide(v1, v2);
}

inline Vector2 operator/(const Vector2& v, const float& f)
{
    return Vector2Scale(v, 1.0f / f);
}

inline Vector2 operator-(const Vector2& v)
{
    return Vector2Negate(v);
}

inline Vector2& operator+=(Vector2& v1, const Vector2& v2)
{
    v1 = Vector2Add(v1, v2);
    return v1;
}

inline Vector2& operator-=(Vector2& v1, const Vector2& v2)
{
    v1 = Vector2Subtract(v1, v2);
    return v1;
}

inline Vector2& operator*=(Vector2& v1, const Vector2& v2)
{
    v1 = Vector2Multiply(v1, v2);
    return v1;
}

inline Vector2& operator*=(Vector2& v, const float& f)
{
    v = Vector2Scale(v, f);
    return v;
}

inline Vector2& operator/=(Vector2& v1, const Vector2& v2)
{
    v1 = Vector2Divide(v1, v2);
    return v1;
}

inline Vector2& operator/=(Vector2& v1, const float& f)
{
    v1 = Vector2Scale(v1, 1.0f / f);
    return v1;
}

inline bool operator==(const Vector2& v1, const Vector2& v2)
{
    return Vector2Equals(v1, v2);
}

inline bool operator!=(const Vector2& v1, const Vector2& v2)
{
    return !Vector2Equals(v1, v2);
}

#pragma endregion //Vector2

inline Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Vector3Add(v1, v2);
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return Vector3Subtract(v1, v2);
}

inline Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
    return Vector3Multiply(v1, v2);
}

inline Vector3 operator*(const Vector3& v1, const float& f)
{
    return Vector3Scale(v1, f);
}

inline Vector3 operator/(const Vector3& v1, const Vector3& v2)
{
    return Vector3Divide(v1, v2);
}

inline Vector3 operator/(const Vector3& v1, const float& f)
{
    return Vector3Scale(v1, 1.0f / f);
}

inline Vector3 operator-(const Vector3& v)
{
    return Vector3Negate(v);
}

inline Vector3& operator+=(Vector3& v1, const Vector3& v2)
{
    v1 = Vector3Add(v1, v2);
    return v1;
}

inline Vector3& operator-=(Vector3& v1, const Vector3& v2)
{
    v1 = Vector3Subtract(v1, v2);
    return v1;
}

inline Vector3& operator*=(Vector3& v1, const Vector3& v2)
{
    v1 = Vector3Multiply(v1, v2);
    return v1;
}

inline Vector3& operator*=(Vector3& v, const float& f)
{
    v = Vector3Scale(v, f);
    return v;
}



#endif //RAYMATHEX_H
