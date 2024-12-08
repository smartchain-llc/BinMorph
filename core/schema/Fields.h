#pragma once
#include <json.hpp>
#include <type_traits>
#include <variant>

namespace bm{

template<typename E> class Endian {
protected:
    enum class EndianType {
        LITTLE, BIG
    };
    Endian() = default;
public:
    // Endian(const Endian& type) = delete;
    // Endian operator=(const Endian&) = delete;
    void read(uint8_t* data, uint8_t* dest, const std::size_t& length){
        return static_cast<E&>(*this).read(data, dest, length);
    }
    EndianType type() const noexcept {
        return static_cast<E&>(*this).type();
    }
};


class LittleEndian : public Endian<LittleEndian>{
public:
LittleEndian() : Endian(){}
    void read(uint8_t* data, uint8_t* dest, const std::size_t& length){
        for(auto i = 0; i < length; i++)
            dest[i] = data[i];
    }
    EndianType type() const noexcept { return EndianType::LITTLE; }
};
class BigEndian : public Endian<LittleEndian>{
public:
BigEndian() : Endian(){}
    void read(uint8_t* data, uint8_t* dest, const std::size_t& length){
        std::size_t srcIndex = length;
        for(auto i = 0; i < length; i++)
            dest[i] = data[--srcIndex];
    }
    EndianType type() const noexcept { return EndianType::BIG; }
};


class Field{
public:
    using EndianTypePtr = std::variant<std::shared_ptr<LittleEndian>, std::shared_ptr<BigEndian>>;
    Field(const nlohmann::json& json);
    void read(uint8_t*);
private:
    std::string _m_name;
    std::size_t _m_length;
    EndianTypePtr _m_endian;
    uint8_t* _m_field_data { nullptr };
    
private:
    void setEndian(const std::string &);
};

}


void SetEndian(const std::string& jsonValue);