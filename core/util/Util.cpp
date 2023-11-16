#include "Util.h"

glm::mat4 GLMMat4FromMatrix(const Matrix& matrix)
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
