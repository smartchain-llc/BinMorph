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
        ss << "0x" << std::hex << std::setw(2) << std::setfill('0') << (unsigned short)results.buffer[i] << " ";
        
        if(i % 15 == 0) ss << "\n";
    }
    std::cout << ss.str() << std::endl;
}

#include <sys/mman.h>
#include <fcntl.h>
TEST(MapperImpl, SharedMemMapperImpl){
    struct SharedMemMapper{
        using ResultsType = void*;
        SharedMemMapper(const char* id): _id{id}{
            _memFd = shm_open(_id, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            if(_memFd == -1)
                throw "shm err";
        }
        uint8_t byteLenOf(std::size_t num){
            if(num == 0) return 1;
            uint8_t byteData[8] = { 0 };
            memcpy(byteData, &num, sizeof(std::size_t));

            uint8_t ret { 8 };
            for(auto i = 8; byteData[i] == 0x00; i--){
                ret = i;
            }
            return ret;
        }
        ResultsType map(bm::Schema s, char* src, const std::size_t& len){
            std::size_t memLen { len };
            ftruncate(_memFd, memLen);
            _addr = mmap(NULL, memLen, PROT_WRITE, MAP_SHARED, _memFd, 0);
            // memcpy(_addr, src, len);
            char* buffer[1024];
            std::size_t index { 0 };
            uint8_t byteLen;
            for(const auto& part: s){
                auto sPart = std::bit_cast<char*>(&part.offset);
                memcpy((char*)_addr+index, part.id.c_str(), part.id.length());
                index += part.id.length();

                byteLen = byteLenOf(part.length);
                memcpy(_addr+index, std::bit_cast<char*>(&part.length), byteLen);
                index += byteLen;
                byteLen = byteLenOf(part.offset);
                memcpy(_addr+index, std::bit_cast<char*>(&part.offset), byteLenOf(part.offset));
                index += byteLen;
            }
            return _addr;
        }
        void* _addr;
        int _memFd;
        const char* _id;
    };
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
    SharedMemMapper mapper {"bmsh1"};
    const auto results = bm::Map<SharedMemMapper>(mapper, schema, _d, 1024);

    ASSERT_EQ(mapper._id, "bmsh1");
}