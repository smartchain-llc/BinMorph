#include <schema/Fields.h>

#include <iostream>
using namespace bm;

template<typename E>
void read(const Endian<E>& endian, uint8_t* data){
    
}

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
    if(jsonValue == "big") { 
        _m_endian = BigEndian();
        
        // reader = Reader(BigEndian{});
    }
    else { 
        _m_endian = LittleEndian();
        
        // reader = Reader(LittleEndian{});
    };
}
