
#include <schema/data/Endian.h>

#include <unordered_map>

using namespace bm::data;
using namespace bm::json;

// LittleEndian.h
struct LittleEndian
{
    static constexpr std::string Type = std::string("little");
    void read(uint8_t *data, uint8_t *dest, const std::size_t &length) const
    {
        for (auto i = 0; i < length; i++)
            dest[i] = data[i];
    }
};

// BigEndian.h
struct BigEndian
{
    static constexpr std::string Type = std::string("big");
    void read(uint8_t *data, uint8_t *dest, const std::size_t &length) const
    {
        std::size_t srcIndex = length;
        for (auto i = 0; i < length; i++)
            dest[i] = data[--srcIndex];
    }
};

std::unordered_map<std::string, std::shared_ptr<Endian>> Endianess = {
    {"big", std::make_shared<Endian>(BigEndian{})},
    {"little", std::make_shared<Endian>(LittleEndian{})}
};

std::shared_ptr<Endian> GetEndian(const FieldAttribute& attribute){
    return Endianess.at(attribute.endian);
}