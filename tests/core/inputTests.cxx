#include <gtest/gtest.h>
#include <bm.h>

#include <TestUtils.h>

using namespace bm;

struct ValidatorStub{
    static InputFile::Status validate(const std::filesystem::path& p){
        return { .is_valid = true, .msg = "" };
    }
};


TEST(FilesystemInput, CanBeInstantiantedWithInitializerList){
    const auto inputPath = "/tmp/schemas";
    const auto inputFile = "test.json";

    const auto inf = InputFile::create<ValidatorStub>({inputPath, inputFile});
    ASSERT_EQ(inf.path(), "/tmp/schemas/test.json");
}

struct DNEValidator {
    static InputFile::Status validate(const std::filesystem::path& p){
        return { false, "File does not exist" };
    }
};
TEST(FilesystemInput, ShouldSetFileStatusToAccordinglyFromValidator){
    const auto doesNotExist = "/tmp/dne.json";
    const auto inFile = InputFile::create(doesNotExist);
    ASSERT_EQ(inFile.status().is_valid, false);
    ASSERT_EQ(inFile.status().msg, "File does not exist");
}
