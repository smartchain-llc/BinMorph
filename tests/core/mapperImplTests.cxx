#include <gtest/gtest.h>

#include <bm.h>

#include <TestUtils.h>

struct CharBuffToStringMapper
{
    using ResultsType = std::string;

    ResultsType map(bm::Schema s, char *d, const std::size_t &len)
    {
        std::stringstream ss;
        for (auto i = 0; i < len; i++)
            ss << "0x" << std::hex << (short)d[i] << " ";
        return {ss.str()};
    }
};
TEST(ConcreteMapperImpl, AbidesByProviderConstraints)
{
    char dataToMap[32];
    auto schema = bm::create_schema(R"(
{
    "p1": {
        "offset": 0,
        "fields":[{"name": "p1.header","length": 8, "endian":"big"}]
    }
}
)");

    const auto results = bm::Map<CharBuffToStringMapper>(schema, dataToMap, 32);
    ASSERT_EQ(typeid(results).name(), typeid(CharBuffToStringMapper::ResultsType).name());

    static_assert(bm::traits::is_mappable<bm::ToJSONMapper, bm::Schema, char *, std::size_t>);
    const auto json_results = bm::Map<bm::ToJSONMapper>(schema, dataToMap, 32);
    std::cout << json_results << std::endl;
}
struct Data{
    Data(std::size_t Len): buffer{ new char[Len] }{}
    ~Data(){ delete[] buffer; }
    char* buffer = nullptr;
};
struct MultiReadMapper{
    using ResultsType = Data;
    static constexpr short BUFF_LEN = 64;

    ResultsType map(bm::Schema s, char* src, const std::size_t& len){
        auto buff = Data(len);
        std::size_t _bytesLeft { len };
        std::size_t _readIndex { 0 };

        while(_bytesLeft > 0){
            auto dataLen =_bytesLeft >= BUFF_LEN ? BUFF_LEN : BUFF_LEN - _bytesLeft;

            memcpy((buff.buffer+_readIndex), src+_readIndex, dataLen);

            _readIndex += dataLen;
            _bytesLeft -= BUFF_LEN;
        }
        return buff;
    }
};
TEST(MapperImpl, HandlesBinDataSizeGTMaxBufferSize){
    auto _d = test_utils::GenerateBinaryData<1024>();
    auto writei = test_utils::WriteStrategy<1024>([](const std::size_t& i, uint8_t& c){ c = i;});
    _d += writei;
    char data[1024];    
    auto schema = bm::create_schema(R"(
{
    "p1": {
        "offset": 0,
        "fields":[{"name": "p1.header","length": 8, "endian":"big"},{"name": "p1.body","length": 16, "endian":"big"}]
    },
    "payload": {
        "offset": 24,
        "fields":[{"name": "data","length": 1000, "endian":"big"}]
    }
}
)");
    const auto results = bm::Map<MultiReadMapper>(schema, _d, 1024);
    std::stringstream ss;
    for(auto i = 0; i < 1024; i++){
        ss << "0x" << std::hex << (short)results.buffer[i] << " ";
        
        if(i % 16 == 0) ss << "\n";
    }
    std::cout << ss.str() << std::endl;
}