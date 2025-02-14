#pragma once
#include <json.hpp>
#include <iostream>
#include <variant>
#include <functional>
namespace bm
{
    template <typename T>
    struct Attribute
    {
        Attribute(T t) : get{
                             [&]() -> decltype(auto)
                             {
                                 return value;
                             }} {}
        T value;
        std::function<T &()> get;
    };

    template <typename... Attributes>
    struct AttributeSpecification
    {
        AttributeSpecification(std::initializer_list<std::pair<std::string, std::variant<Attributes...>>> att)
        {
            for (const auto &[id, value] : att)
            {
                _attMap[id] = value;
            }
        }
        inline std::size_t attribute_count() const noexcept { return _attMap.size(); }
        std::map<std::string, std::variant<Attributes...>> _attMap;
    };
    struct FieldAttribute
    {
        FieldAttribute(const nlohmann::json &json)
        {
            json["name"].get_to(name);
            json["length"].get_to(length);
            json["endian"].get_to(endian);
        }
        std::size_t length;
        std::string name;
        std::string endian;

        static bool containsRequired(const nlohmann::json &json)
        {
            if (!json.contains("name"))
            {
                return false;
            }
            if (!json.contains("length"))
            {
                return false;
            }
            if (!json.contains("endian"))
            {
                return false;
            }
            return true;
        }
    };
    void from_json(const nlohmann::json &json, FieldAttribute &att);

    struct DefaultLayoutSpec{
        static bool containsProperLayout(const nlohmann::json &json)
        {
            if(!json.contains("offset"))
            {
                std::cerr << "Layout element 'offset' missing\n";
                return false;
            }
            if (!json.contains("fields"))
            {
                std::cerr << "Layout element 'fields' missing\n";
                return false;
            }
            return true;
        } 
    };
    struct ProceduralOffsetLayouts
    {
        static bool containsProperLayout(const nlohmann::json &json)
        {
            if (!json.contains("fields"))
            {
                std::cerr << "Layout element 'fields' missing\n";
                return false;
            }
            return true;
        }
    };
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
        LayoutAttribute() {}
        LayoutAttribute(const nlohmann::json &json) { __init(json); }
        // LayoutAttribute(nlohmann::json &json, const std::size_t& offset) {
        //     nlohmann::json _w_offset = nlohmann::json::parse(json);
        //     _w_offset["offset"] = offset;
        //     _w_offset.emplace_back()
        //     // for(const auto&[key,value] : json.items())
        //     //     _w_offset[key] = value;
        //     std::cout << "PROC JSON: \n"<<_w_offset<<std::endl;
        //     __init(_w_offset); 
        // }
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
        bool operator<(const LayoutAttribute& rhs) const noexcept{ return offset < rhs.offset; }
        bool overlaps(const LayoutAttribute &l) const noexcept
        {
            // start, end
            // this: 0, 100
            // l   : 50, 200
            if (offset < l.endOffset())
                return true;
            return false;
        }
        template<typename Spec = DefaultLayoutSpec>
        static bool containsProperLayout(const nlohmann::json &json)
        {
            return Spec::containsProperLayout(json);
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
            bool _overlaps = lhs < rhs ? rhs.overlaps(lhs) : lhs.overlaps(rhs);
            if (_overlaps)
                throw "Layout offsets overlap";
            return lhs < rhs;
        }
    };
    // why multiple def here?
}
