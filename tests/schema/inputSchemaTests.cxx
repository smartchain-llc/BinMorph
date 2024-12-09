#include <gtest/gtest.h>
#include <TestUtils.h>

#include <filesystem>
#include <iostream>
#include <fstream>

#include <schema/schema.h>
#include <schema/data/Fields.h>

using namespace bm;

static auto SimpleJSONSchema = test_utils::JSONFrom("simple");

struct SchemaData{
    SchemaData(const Schema& s){
        std::size_t fieldOffset {0};
        for(const auto& fieldJSON : s.__fields()){
            data.emplace_back(fieldJSON);
            offsetData.insert({fieldOffset, Field{fieldJSON}});    
            fieldOffset += fieldJSON.length;        
        }
    }
    std::vector<Field> data;
    std::unordered_map<std::size_t, Field> offsetData;
};

TEST(InputSchema, ParsesAllFieldAttributes){
    auto schema = Schema{ SimpleJSONSchema };
    ASSERT_EQ(schema.__fields().size(), 3);
}


struct Reader{

};

TEST(SchemaData, ParsesAllFields){
    auto schema = Schema{ SimpleJSONSchema };
    auto sdata = SchemaData(schema);

    auto data = test_utils::GenerateBinaryData<1024>();
    auto WriteIndexValue = test_utils::WriteStrategy<16>{[](const std::size_t& i, uint8_t& c){ 
        c = i; 
    }};
    auto WriteIfMod_5 = test_utils::WriteStrategy<257>{[](const std::size_t& i, uint8_t& c){ 
        if(i % 5 == 0) c = i;
        else c = 0xFF; 
    }};

    data += WriteIndexValue;
    data += WriteIfMod_5;
    uint8_t buffer[1024];
    for(const auto&[offset, field] : sdata.offsetData){
        std::cout << "["<<offset<<"] " << field.name() << ":" << field.length() << std::endl;
        field.endian()->read(&data[offset], &buffer[offset], field.length());

    }

}

// /**************************************************
// == TESTS: SchemaFileState
// **************************************************/
// TEST(SchemaFileState, CanCompareStateAgainstStatus){
//     const auto v = SchemaFileState::ValidState();
//     ASSERT_TRUE(v == SchemaFileStatus::VALID);
// }

// /**************************************************
// == TESTS: SchemaFile
// **************************************************/

// TEST(SchemaFile, HasInvalidStateOnInitFailure){
//     SchemaFile f{""};
//     ASSERT_FALSE(f.isValid());
// }

// /**************************************************
// == TESTS: SchemaFile (Fixture)
// **************************************************/

// class SchemaFile_F : public testing::Test {
// protected:
//     SchemaFile_F():s{test_utils::toPath({TEST_SCHEMAS_PATH, "simple.json"})}{}
//     SchemaFile s;
// };

// TEST_F(SchemaFile_F, PopualatesJSONDataMember){
//     ASSERT_TRUE(s.json() != nullptr);
//     ASSERT_TRUE(s.field(0) != nullptr);
//     ASSERT_EQ(s.field(0)["name"], "header");
// }

// TEST(SchemaFile, PopulatesJSONSchemaDataMember){
//     SchemaFile file{ test_utils::toPath({TEST_SCHEMAS_PATH, "simple.json"})};
//     auto fields = file.json()["fields"];
//     ASSERT_TRUE(fields != nullptr);
//     ASSERT_EQ(fields[0]["name"], "header");
// }

// TEST(SchemaFileJSON, CanValidateBinDataWithSchema){
//     SchemaFile file{ test_utils::toPath({TEST_SCHEMAS_PATH, "simple.json"})};
//     auto fields = file.json()["fields"];

//     auto headerJSON = fields[0]["name"];
//     ASSERT_EQ(headerJSON, "header");
//     auto headerType = fields[0]["type"];
//     auto headerEndianess = fields[0]["endian"];

//     uint16_t value = headerEndianess == "big"
//         ? (test_utils::data::Simple[0] << 8) | (test_utils::data::Simple[1])
//         : (test_utils::data::Simple[1] << 8) | (test_utils::data::Simple[0]);
//     ASSERT_EQ(value, 258);
// }
// TEST(SchemaFileJSON, CanIndexJSONFromSchemaFileObject){

// }