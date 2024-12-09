#include <gtest/gtest.h>
#include <TestUtils.h>

#include <schema/data/Fields.h>
// #include <schema/json/FieldAttribute.h>

using namespace bm;

static auto SimpleJSONSchema = test_utils::JSONFrom("simple");

TEST(FieldAttribute, ParsesCorrectEndianessFromJSON){
    std::vector<nlohmann::json> fields;
    for(const auto& [key, value] : SimpleJSONSchema.items()){
        value.template get_to(fields);
    }

    const auto fa = FieldAttribute(fields[0]);
    ASSERT_EQ(fa.name, "header");
    ASSERT_EQ(fa.length, 16);

    const auto fieldData = Field(fa);
    ASSERT_EQ(fieldData.name(), "header");
    ASSERT_EQ(fieldData.endian()->type(), "big");
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

    const Field fdata { FieldAttribute{fields[0]} };
    uint8_t edata[256];
    fdata.endian()->read(data, edata, fdata.length());

    ASSERT_EQ(edata[0], 15);
    ASSERT_EQ(edata[15], 0);
}