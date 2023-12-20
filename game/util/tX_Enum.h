#ifndef HORDE_TX_ENUM_H
#define HORDE_TX_ENUM_H

#include <algorithm>
#include <string>
#include "tX_TemplateHelpers.h"
#include "defines.h"
#include <unordered_map>

namespace tX
{

template<EnumType T>
class Enum
{
private:
    T Value;
    std::unordered_map<std::string, T> NamesToValues;
    std::unordered_map<T, std::string> ValuesToNames;
public:
    Enum() = default;
    Enum(T value) : Value(value) {}
    Enum(const Enum& other) : Value(other.Value) {}
    Enum(Enum&& other) noexcept : Value(std::move(other.Value)) {}

    Enum& operator=(const Enum& other)
    {
        Value = other.Value;
        return *this;
    }

    Enum& operator=(Enum&& other) noexcept
    {
        Value = std::move(other.Value);
        return *this;
    }

    operator T() const { return Value; }

    bool operator==(const Enum& other) const { return Value == other.Value; }
    bool operator!=(const Enum& other) const { return Value != other.Value; }

    bool operator==(const T& other) const { return Value == other; }
    bool operator!=(const T& other) const { return Value != other; }

    bool operator==(const i8& other) const { return Value == other; }
    bool operator!=(const i8& other) const { return Value != other; }

    bool operator==(const u8& other) const { return Value == other; }
    bool operator!=(const u8& other) const { return Value != other; }

    bool operator==(const i16& other) const { return Value == other; }
    bool operator!=(const i16& other) const { return Value != other; }

    bool operator==(const u16& other) const { return Value == other; }
    bool operator!=(const u16& other) const { return Value != other; }

    bool operator==(const i32& other) const { return Value == other; }
    bool operator!=(const i32& other) const { return Value != other; }

    bool operator==(const u32& other) const { return Value == other; }
    bool operator!=(const u32& other) const { return Value != other; }

    bool operator==(const i64& other) const { return Value == other; }
    bool operator!=(const i64& other) const { return Value != other; }

    bool operator==(const u64& other) const { return Value == other; }
    bool operator!=(const u64& other) const { return Value != other; }
};

}

#endif //HORDE_TX_ENUM_H
