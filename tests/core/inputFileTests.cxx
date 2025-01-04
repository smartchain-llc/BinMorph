#include <gtest/gtest.h>
#include <bm/input.h>
#include <bm/schema.h>
#include <TestUtils.h>


struct ThrowableFileValidator
{
    bool validate(const std::filesystem::path &f)
    {
        throw "Throwable Error";
        return true;
    }
};
struct ThrowableInputFile : public bm::InputFile<ThrowableFileValidator>{
    
};
TEST(ThrowableValidator, ThrowsOnInvalidInput)
{
    auto inPath = std::filesystem::path(TEST_SCHEMAS_PATH);
    // ASSERT_ANY_THROW([&](){ auto schema = bm::SchemaFile<ThrowableFileValidator>(inPath); });
}

TEST(InputFile, IsImplicitlyConvertableToSTLPath)
{
    auto inFile = std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json";
    bm::SchemaFile schema{inFile};
    static_assert(std::convertible_to<bm::InputFile<bm::DefaultFileValidator>, std::filesystem::path>);
}
TEST(DefaultValidator, InvalidatesOnDirectoryInput)
{
    auto inPath = std::filesystem::path(TEST_SCHEMAS_PATH);
    bm::SchemaFile schema{inPath};
    ASSERT_FALSE(schema.isValid());
}
TEST(DefaultValidator, InvalidatesOnEmptyInputFile)
{
    auto inPath = std::filesystem::path(TEST_SCHEMAS_PATH) / "empty.json";
    bm::SchemaFile schema{inPath};
    ASSERT_FALSE(schema.isValid());
}

// TEST(InputFile, FileValidatorDefaultsToDefaultFileValidator)
// {
//     auto inFile = std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json";
//     bm::SchemaFile<bm::SchemaFileValidator> schema{};
//     bm::SchemaFile schema{inFile};
//     ASSERT_TRUE(schema.isValid());
// }