#pragma once
#include <schema/Specification.h>

namespace bm{

class Header: public Specification{
public:
    static constexpr std::string Key = "header";
public:

public:
    Header(): Specification("header"){}
    void setData(const nlohmann::json& data){
        data.at("length").get_to(m_length_);
    }
    inline auto length() const noexcept { return m_length_; }
private:
    std::size_t m_length_ { 0 };
};

#include <vector>
struct HeaderField{
friend class Header;
    std::string name;
    std::size_t length;
    std::string endian;
};
void from_json(const nlohmann::json& headerData, HeaderField& field){
    headerData.at("name").get_to(field.name);
    headerData.at("length").get_to(field.length);
    headerData.at("endian").get_to(field.endian);
}
struct HeaderData{
    std::size_t headerLength;
    std::vector<HeaderField> fields;
};
void from_json(const nlohmann::json& data, HeaderData& header){
    data.at("length").get_to(header.headerLength);
    data.at("fields").get_to(header.fields);
}
}