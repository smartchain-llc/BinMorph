#include <schema/Fields.h>

#include <iostream>
using namespace bm;
using EndianTypePtr = std::variant<std::shared_ptr<LittleEndian>, std::shared_ptr<BigEndian>>;
using EndianMapType = std::unordered_map<
    std::string,
    EndianTypePtr
>;

static const EndianMapType EndianType = {
    {"big", std::make_shared<BigEndian>()},
    {"little", std::make_shared<LittleEndian>()}
};


void SetEndian(const std::string &jsonValue, std::variant<LittleEndian, BigEndian>& endian)
{
    if(jsonValue == "big") endian = BigEndian();
    endian = LittleEndian();
}

void t(const Field& f)
{
    
}

Field::Field(const nlohmann::json &json)
{
    json["name"].get_to(_m_name);
    json["length"].get_to(_m_length);

    const auto endianType = json["endian"].get<std::string>();
    setEndian(endianType);
}

void Field::setEndian(const std::string &jsonValue){
    _m_endian = EndianType.at(jsonValue);
}
