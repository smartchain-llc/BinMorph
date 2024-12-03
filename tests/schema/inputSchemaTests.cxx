#include <gtest/gtest.h>
#include <TestUtils.h>
#include <fsUtils.h>

#include <schema/SchemaFile.h>

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace bm;

/**************************************************
== TESTS: SchemaFileState
**************************************************/
TEST(SchemaFileState, CanCompareStateAgainstStatus){
    const auto v = SchemaFileState::ValidState();
    ASSERT_TRUE(v == SchemaFileStatus::VALID);
}

/**************************************************
== TESTS: SchemaFile
**************************************************/

TEST(SchemaFile, HasInvalidStateOnInitFailure){
    SchemaFile f{""};
    ASSERT_FALSE(f.isValid());
}

/**************************************************
== TESTS: SchemaFile (Fixture)
**************************************************/

class SchemaFile_F : public testing::Test {
protected:
    SchemaFile_F():s{test_utils::toPath({TEST_SCHEMAS_PATH, "simple.json"})}{}
    SchemaFile s;
};

TEST_F(SchemaFile_F, PopualatesJSONDataMember){
    ASSERT_TRUE(s.json() != nullptr);
    ASSERT_TRUE(s.field(0) != nullptr);
    ASSERT_EQ(s.field(0)["name"], "header");
}

TEST(SchemaFile, PopulatesJSONSchemaDataMember){
    SchemaFile file{ test_utils::toPath({TEST_SCHEMAS_PATH, "simple.json"})};
    auto fields = file.json()["fields"];
    ASSERT_TRUE(fields != nullptr);
    ASSERT_EQ(fields[0]["name"], "header");
}

TEST(SchemaFileJSON, CanValidateBinDataWithSchema){
    SchemaFile file{ test_utils::toPath({TEST_SCHEMAS_PATH, "simple.json"})};
    auto fields = file.json()["fields"];

    auto headerJSON = fields[0]["name"];
    ASSERT_EQ(headerJSON, "header");
    auto headerType = fields[0]["type"];
    auto headerEndianess = fields[0]["endian"];

    uint16_t value = headerEndianess == "big"
        ? (test_utils::data::Simple[0] << 8) | (test_utils::data::Simple[1])
        : (test_utils::data::Simple[1] << 8) | (test_utils::data::Simple[0]);
    ASSERT_EQ(value, 258);
}
TEST(SchemaFileJSON, CanIndexJSONFromSchemaFileObject){

}
// using json = nlohmann::json;

// struct SchemaFile{
//     SchemaFile(const std::filesystem::path& filePath): m_path_{std::move(filePath)}{
//         m_in_stream_ = std::ifstream(m_path_);

//     }
//     SchemaFile(std::filesystem::path&& filePath): SchemaFile(filePath){}

//     const std::filesystem::path& path() const noexcept { return m_path_; }
//     std::ifstream& stream() { return m_in_stream_; }
//     [[nodiscard]] operator std::ifstream&() noexcept { return m_in_stream_; }
//     std::filesystem::path m_path_;
//     std::ifstream m_in_stream_;
// };

// TEST(Schema_TEST_SETUP, CanFindSchemaTestFile){
//     const auto file = toPath({TEST_SCHEMAS_PATH, "simple.json"});
//     SchemaFile inSchema { file };
//     ASSERT_TRUE(std::filesystem::exists(inSchema.path()));
//     ASSERT_TRUE(inSchema.m_in_stream_.is_open());
// }

// TEST(SchemaFile, CanPassDataToJSONParser){
//     const auto file = toPath({TEST_SCHEMAS_PATH, "simple.json"});
//     SchemaFile inSchema { file };
//     const auto schemaData = json::parse(inSchema);
    
// }