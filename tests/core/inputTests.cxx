#include <gtest/gtest.h>
#include <bm.h>

#include <TestUtils.h>

using namespace bm;

struct ValidatorStub{
    static file::Status validate(const std::filesystem::path& p){
        return file::Status{ true, "" };
    }
};


TEST(FilesystemInput, CanBeInstantiantedWithInitializerList){
    const auto inputPath = "/tmp/schemas";
    const auto inputFile = "test.json";

    const auto inf = file::FileFactory::create<ValidatorStub>({inputPath, inputFile});
    ASSERT_EQ(inf.path(), "/tmp/schemas/test.json");
}

struct DNEValidator {
    static file::Status validate(const std::filesystem::path& p){
        return { false, "File does not exist" };
    }
};
TEST(FilesystemInput, ShouldSetFileStatusToAccordinglyFromValidator){
    const auto doesNotExist = "/tmp/dne.json";
    const auto inFile = file::FileFactory::create(doesNotExist);
    ASSERT_EQ(inFile.status().is_valid, false);
    ASSERT_EQ(inFile.status().msg, "File does not exist");
}

struct V{ static file::Status validate(const std::filesystem::path& path){ return file::Status(); }};
TEST(CleanerFile, CanDefineSpecificValidators){
    static_assert(file::traits::is_file_validator<V>);
}
struct StatusFail{ static file::Status validate(const std::filesystem::path& path){ return file::Status(false,"AlwaysFails"); }};
struct StatusThrows{ static file::Status validate(const std::filesystem::path& path){ throw ""; }};
TEST(File, CanOnlyBeConstructedFromFactory){
    // const auto file = file::File(test_utils::JSON_Test_Schemas.at("256"), {}); // OK
    const auto file = file::FileFactory::create(test_utils::JSON_Test_Schemas.at("256"));
    const auto failStatus = file::FileFactory::create<StatusFail>(test_utils::JSON_Test_Schemas.at("256"));
    ASSERT_EQ(failStatus.status().is_valid, false);
    ASSERT_ANY_THROW(file::FileFactory::create<StatusThrows>(test_utils::JSON_Test_Schemas.at("256")));
}