#include <gtest/gtest.h>
#include <bm/input.h>
#include <bm/schema.h>
#include <TestUtils.h>

template<bm::traits::DataProvider D> 
void use(D& dp){
    uint8_t buf[dp.length()];
    dp.readTo(buf, dp.length());
    ASSERT_TRUE(buf != nullptr);
}
struct FileStreamProvider{
    FileStreamProvider(std::filesystem::path p): _path{p}{}
    void readTo(uint8_t* dest, std::size_t len){
        std::ifstream in {_path};
        in.read((char*)dest, len);
    }
    std::size_t length() { return std::filesystem::file_size(_path); }
    std::filesystem::path _path;
};
TEST(DataProviderConcept, CanReadFromMultiBinDataSources){
    std::filesystem::path in {std::filesystem::path(TEST_DATA_PATH) / "256.bin" };
    FileStreamProvider inFile { in };
    // use(inFile);
}