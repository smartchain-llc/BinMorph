#pragma once
#include <json.hpp>

namespace bm
{
    struct FieldAttribute
    {
        FieldAttribute() = default;
        FieldAttribute(const nlohmann::json &json)
        {
            json["name"].get_to(name);
            json["length"].get_to(length);
            json["endian"].get_to(endian);
        }
        std::size_t length;
        std::string name;
        std::string endian;
    };
    void from_json(const nlohmann::json& json, FieldAttribute& att);

    /**
        ### Schema JSON Attribute: BinaryLayout
        A json object with an 'offset' attribute and 'fields' attribute.

        Attribute readers should start reading from the offset and interpret the data
        as described by the number of fields.
    */
    struct LayoutAttribute
    {
        void __init(const nlohmann::json &json)
        {
            json.at("offset").get_to(offset);
            
            for (const auto &fieldsJSON : json.at("fields"))
            {
                auto attribute = FieldAttribute{fieldsJSON};
                fields.push_back(attribute);
                length += attribute.length;
            }
        }
        // Remove Default Cnstr
        LayoutAttribute(){}
        LayoutAttribute(const nlohmann::json &json) { __init(json); }
        LayoutAttribute(const std::string &id, const nlohmann::json &json) : id{id}
        {
            __init(json);
        }
        std::vector<FieldAttribute> fields;
        std::string id;
        std::size_t offset{0};
        std::size_t length{0};

        inline const std::size_t &byteLength() const noexcept { return length; }
        inline const std::size_t &startOffset() const noexcept { return offset; }
        inline const std::size_t endOffset() const noexcept { return length + offset; }
        bool overlaps(const LayoutAttribute& l) const noexcept {
            // start, end
            // this: 0, 100
            // l   : 50, 200
            if(l.endOffset() < offset)
                return true;
            return false;
        }
    };
    struct LayoutComparator
    {
        constexpr bool operator()(const LayoutAttribute &lhs, const LayoutAttribute &rhs) const
        {
            return lhs.offset < rhs.offset;
        }
    };
    struct LayoutOverlapComparator
    {
        const bool operator()(const LayoutAttribute &lhs, const LayoutAttribute &rhs) const
        {
            if(lhs.overlaps(rhs))
                throw "Layout offsets overlap";
            return lhs.offset < rhs.offset;
        }
    };
    // why multiple def here?
}
