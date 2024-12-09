#pragma once
#include <schema/data/Endian.h>
#include <type_traits>
#include <variant>

namespace bm::data
{

    class Field
    {
    public:
        Field(const json::FieldAttribute &data);
        inline const std::size_t &length() const noexcept { return _m_length; }
        inline std::string_view name() const noexcept { return _m_name; }
        inline const std::shared_ptr<Endian> endian() const noexcept { return _m_endian; }

    private:
        std::shared_ptr<Endian> _m_endian;
        std::string _m_name;
        std::size_t _m_length;
        uint8_t *_m_field_data{nullptr};

    private:
        void setEndian(const std::string &);
    };

}