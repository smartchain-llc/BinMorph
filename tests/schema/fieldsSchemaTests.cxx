#include <gtest/gtest.h>
#include <TestUtils.h>

#include <schema/data/Fields.h>
// #include <schema/json/FieldAttribute.h>

using namespace bm;

static auto SimpleJSONSchema = test_utils::JSONFrom("simple");
/**********************************************************************
TEST: Field Attribute Class
**********************************************************************/
// namespace _bm
// {
//     struct Field
//     {
//         Field() = default;
//         Field(int i){}
//         std::size_t length;
//         std::string name;
//         std::string endian;
//     };
//     void from_json(const nlohmann::json& json, Field& f){
//     json["name"].get_to(f.name);
//     json["length"].get_to(f.length);
//     json["endian"].get_to(f.endian);

//     }
// }
// TEST(FieldAttribute, CanUtilize_from_json_FN)
// {
//     const auto json = nlohmann::json::parse(R"(
//     {
//         "fields":[{"name": "l1f1", "length": 16, "endian": "big"}]  

//     }
//     )");
//     auto _l1f1 = json.template get<_bm::Field>();
//     auto l1f1 = json.template get<FieldAttribute>();
//     ASSERT_EQ(l1f1.name, "l1f1");
// }

TEST(FieldAttribute, ParsesCorrectEndianessFromJSON)
{
    std::vector<nlohmann::json> fields;
    for (const auto &[key, value] : SimpleJSONSchema.items())
    {
        value.template get_to(fields);
    }

    const auto fa = FieldAttribute(fields[0]);
    ASSERT_EQ(fa.name, "header");
    ASSERT_EQ(fa.length, 16);

    const auto fieldData = Field(fa);
    ASSERT_EQ(fieldData.name(), "header");
    ASSERT_EQ(fieldData.endian()->type(), "big");
}
TEST(Fields, UtilizesEndianValueToFormatData)
{

    auto data = test_utils::GenerateBinaryData<256>();
    auto WriteIndexValue = test_utils::WriteStrategy<16>{[](const std::size_t &i, uint8_t &c)
                                                         {
                                                             c = i;
                                                         }};
    auto WriteIfMod_5 = test_utils::WriteStrategy<257>{[](const std::size_t &i, uint8_t &c)
                                                       {
                                                           if (i % 5 == 0)
                                                               c = i;
                                                           else
                                                               c = 0xFF;
                                                       }};

    data += WriteIndexValue;
    data += WriteIfMod_5;

    std::vector<nlohmann::json> fields;
    for (const auto &[key, value] : SimpleJSONSchema.items())
    {
        value.template get_to(fields);
    }

    const Field fdata{FieldAttribute{fields[0]}};
    uint8_t edata[256];
    fdata.endian()->read(data, edata, fdata.length());

    ASSERT_EQ(edata[0], 15);
    ASSERT_EQ(edata[15], 0);
}

/**********************************************************************
TEST: Layout Attribute Class
**********************************************************************/
// TEST(LayoutAttribute, CanUtilize_from_json_FN){
//     const auto json = nlohmann::json::parse(R"(
//     {
//         "l1":{
//             "offset": 0,
//             "fields":[{"name": "l1f1", "length": 16, "endian": "big"}]
//         }
//     }
//     )");
//     FieldAttribute att;
//     for(const auto&[key, value] : json.items()){
//         const auto l1f1 = value.at("fields")[0];
//         att = l1f1.template get<bm::FieldAttribute>();
//     }
//     ASSERT_EQ(att.name, "l1f1");
// }