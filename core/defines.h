#ifndef DEFINES_H
#define DEFINES_H

#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

namespace tZ
{

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

using f32 = float;
using f64 = double;

using v2 = glm::vec2;
using v3 = glm::vec3;
using v4 = glm::vec4;

using m4 = glm::mat4;

using quat = glm::quat;

}

#endif //DEFINES_H
