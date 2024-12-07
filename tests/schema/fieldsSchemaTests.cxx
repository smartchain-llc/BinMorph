#include <gtest/gtest.h>
#include <TestUtils.h>

#include <schema/Fields.h>

static auto SimpleJSONSchema = test_utils::JSONFrom("simple");

TEST(Fields, ParsesCorrectEndianessFromJSON){
    std::vector<nlohmann::json> fields;
    for(const auto& [key, value] : SimpleJSONSchema.items()){
        value.template get_to(fields);
    }

    const auto field = bm::Field(fields[0]);

}
TEST(Fields, UtilizesEndianValueToFormatData){

    auto data = test_utils::GenerateBinaryData<256>();
    auto WriteIndexValue = test_utils::WriteStrategy<16>{[](const std::size_t& i, uint8_t& c){ 
        c = i; 
    }};
    auto WriteIfMod_5 = test_utils::WriteStrategy<257>{[](const std::size_t& i, uint8_t& c){ 
        if(i % 5 == 0) c = i;
        else c = 0xFF; 
    }};

    data += WriteIndexValue;
    data += WriteIfMod_5;

    std::vector<nlohmann::json> fields;
    for(const auto& [key, value] : SimpleJSONSchema.items()){
        value.template get_to(fields);
    }

    const auto field = bm::Field(fields[0]);

    // const auto data = field.read(data);

}