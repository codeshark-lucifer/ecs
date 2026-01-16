#include <utils/matrix.h>

mat3::mat3(const mat4 &mat4_)
{
    m[0] = mat4_.m[0];
    m[1] = mat4_.m[1];
    m[2] = mat4_.m[2];
    m[3] = mat4_.m[4];
    m[4] = mat4_.m[5];
    m[5] = mat4_.m[6];
    m[6] = mat4_.m[8];
    m[7] = mat4_.m[9];
    m[8] = mat4_.m[10];
}
