#pragma once
#include <schema/data/Endian.h>
#include <type_traits>
#include <variant>

namespace bm
{
    class Field
    {
    public:
        Field(const FieldAttribute &data);
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
    // void from_json(const nlohmann::json& json, FieldAttribute& f){
    //         json["name"].get_to(name);
    //         json["length"].get_to(length);
    //         json["endian"].get_to(endian);

    // }
}