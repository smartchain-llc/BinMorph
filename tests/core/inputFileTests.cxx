#include <gtest/gtest.h>
#include <bm/input.h>
#include <bm/schema.h>
#include <TestUtils.h>

TEST(InputFile, FileValidatorDefaultsToDefaultFileValidator)
{
    auto inFile = std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json";
    bm::InputFile schema{inFile};
    ASSERT_TRUE(schema.isValid());
}
TEST(InputFile, IsImplicitlyConvertableToSTLPath)
{
    auto inFile = std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json";
    bm::InputFile schema{inFile};
    static_assert(std::convertible_to<bm::InputFile<bm::DefaultFileValidator>, std::filesystem::path>);
}
TEST(DefaultValidator, InvalidatesOnDirectoryInput)
{
    auto inPath = std::filesystem::path(TEST_SCHEMAS_PATH);
    bm::InputFile schema{inPath};
    ASSERT_FALSE(schema.isValid());
}
TEST(DefaultValidator, InvalidatesOnEmptyInputFile)
{
    auto inPath = std::filesystem::path(TEST_SCHEMAS_PATH) / "empty.json";
    bm::InputFile schema{inPath};
    ASSERT_FALSE(schema.isValid());
}
struct ThrowableFileValidator
{
    bool validate(const std::filesystem::path &f)
    {
        throw "Throwable Error";
        return true;
    }
};
TEST(ThrowableValidator, ThrowsOnInvalidInput)
{
    auto inPath = std::filesystem::path(TEST_SCHEMAS_PATH);
    ASSERT_ANY_THROW(bm::InputFile<ThrowableFileValidator> schema{inPath});
}

struct BinaryFile : public bm::InputFile<bm::DefaultFileValidator>
{
    BinaryFile(std::filesystem::path filePath) : bm::InputFile<bm::DefaultFileValidator>(filePath)
    {
    }
};
TEST(BinaryFile, ContainsBinaryDataFromFile)
{
    const auto binFile = std::filesystem::path(TEST_DATA_PATH) / "256.bin";
    const auto schemaFile = std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json";
    bm::SchemaFile inSchema { schemaFile };
    bm::InputFile inBin { binFile };

    std::ifstream in { inBin.filePath() };
    bm::SchemaMapper<bm::ToJSONMapper> mapper;
    // mapper.map(inSchema.get(), in);
}