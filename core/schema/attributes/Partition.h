#pragma once
#include "Segment.h"
#include "FieldAttributes.h"
#include <vector>
#include <string_view>

class Partition{
    Partition(const nlohmann::json& json);

    inline std::string_view   key()    const noexcept { return _m_key; }
    inline const std::size_t& length() const noexcept { return _m_length; }
    inline const std::size_t& start_offset() const noexcept { return _m_offset; }
    inline const std::size_t& end_offset()   const noexcept { return _m_offset + _m_length; }
private:
    std::string _m_key;
    std::size_t _m_length;
    std::size_t _m_offset;
    std::vector<FieldAttribute> _m_fields;
};

template<typename V>
Partition&& CreateFrom(V& v, const nlohmann::json& json);