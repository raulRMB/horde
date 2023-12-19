#ifndef HORDE_TX_TEMPLATEHELPERS_H
#define HORDE_TX_TEMPLATEHELPERS_H

#include <type_traits>

namespace tX
{
template<typename T>
using IsEnum = std::is_enum<T>;

template<typename T>
static inline constexpr bool IsEnumV = IsEnum<T>::value;

template<typename T>
concept EnumType = IsEnumV<T>;
}


#endif //HORDE_TX_TEMPLATEHELPERS_H
