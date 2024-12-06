#include <gtest/gtest.h>
#include <TestUtils.h>
#include <json.hpp>
#include <variant>
#include <unordered_map>
#include <schema/Fields.h>
static auto SimpleJSON = test_utils::JSONFrom("simple");

struct FieldBinData {
    FieldBinData(const nlohmann::json& schema){
        std::cout << schema << std::endl;
        schema["name"].get_to(name);
        schema["length"].get_to(length);
        schema["endian"].get_to(endian);
    }
    std::string name;
    std::size_t length;
    std::string endian;
    FieldBinData& operator<<(uint8_t* data) {
        uint8_t binData[length];
        set(endian, data, binData);
        return *this;
    }
    void set(const std::string& type, uint8_t* src, uint8_t* dest){
        if(type == "big"){
            for(auto i = 0; i < length; i++)
                dest[i] = src[(length-(i+1))];
        }
        else{
            for(auto i = 0; i < length; i++)
                dest[i] = src[i];
        }
    }
};

#include <algorithm>

struct SchemaDataMap{
    
    void map(uint8_t* data, const nlohmann::json& field){
        int index = 0;
        const auto a = field.template get<std::vector<nlohmann::json>>();
        for(auto i = 0; i < a.size(); i++){
            FieldBinData f { a.at(i) };
            f << data;
            offsetMap.push_back({mapByteLength, std::move(f)});
            mapByteLength += f.length; 
        }
    }
    const FieldBinData& from(const std::string& fieldName) const noexcept {
      auto data = std::find_if(offsetMap.begin(), offsetMap.end(), [&](const std::pair<std::size_t, FieldBinData>& item){
            return fieldName == item.second.name;
      });
      return data->second;
    }
    const FieldBinData& from(const std::size_t offset) const noexcept {
        auto data = std::find_if(offsetMap.begin(), offsetMap.end(), [&](const std::pair<std::size_t, FieldBinData>& item){
            return offset == item.second.length;
        });
        return data->second;
    }
    std::vector<std::pair<std::size_t, FieldBinData>> offsetMap;
    std::size_t mapByteLength { 0 };
};

TEST(SchemaDataMap, UseThisForFurtherDevelopment){
    uint8_t data[(16+256+8)];
    auto setData = [&]{
        std::size_t i {0};
        for(; i < 16; i++)
            data[i] = (uint8_t)i;
        for(; i < 256; i++)
            data[i] = (i % 5) ? 0xFF : 0x00;
        for(; i < 8; i++)
            data[i] = 0x1A;
    };
    setData();
    
    SchemaDataMap mappedData;
    for(const auto&[key, value] : SimpleJSON.items())
        mappedData.map(data, value);
    const auto payload = mappedData.from("payload");
    ASSERT_TRUE(payload.name == "payload");
}