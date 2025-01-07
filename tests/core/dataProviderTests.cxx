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
template<typename T> concept is_data_provider = requires(T t, char* dest, std::size_t n){
    {t.read(dest, n)};
};
// template<typename T> requires is_data_provider<T>
struct DataProvider{
    // void read(char* dest, std::size_t len){
    //     static_cast<T&>(*this)->read(dest, len);
    // }
};

// struct StreamProvider : public DataProvider<StreamProvider>{

// };
// struct InlineData : public DataProvider<InlineData>{
struct InlineData{
    InlineData(){
        auto tmp = test_utils::GenerateBinaryData<256>();
        auto write_zeros = test_utils::WriteStrategy<256>([](const std::size_t& i, uint8_t& c){
            c = 0x00;
        });
        tmp += write_zeros;
        memcpy(data, tmp(), 256);
    }
    void read(char* dest, std::size_t n){
        for(auto i = 0; i < n; i++)
            dest[i] = data[i];
    }
    uint8_t data[256];
};

template <typename MapperImpl, bm::clean::is_schema_type S, typename Data>
bm::Results<typename MapperImpl::ResultsType> Map(S s, Data d){
    uint8_t buffer[1024];
    // d.read()
    for(const auto& partition : s){

    }
}
template<is_data_provider T>
void provider_user(T& in){

    uint8_t buffer[256];
    in.read((char*)buffer, 256);
}
struct InvalidProvider {};
TEST(DataProviderTraits, ProvidesInterfaceForReadingInternalBytesToBuffer){
    static_assert(is_data_provider<InlineData>);
    InlineData data;
    InvalidProvider invalid;
    provider_user(data);
}
TEST(DataProviderTraits, IsSimple){
    const auto schema = bm::create_schema(R"(
    {
        "1":{"offset":0,"fields":[{"name":"1.1", length:8, endian:"big"}]}
    }
    )");
    // InlineData data{0xff, 0x00};
    // Map<bm::mappers::ToJSONMapper>(schema, data);
}