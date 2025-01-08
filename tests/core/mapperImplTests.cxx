#include <gtest/gtest.h>

#include <bm/schema.h>

#include <TestUtils.h>

static auto GeneratedData = [](const std::size_t &len, uint8_t *dest)
{
    for (auto i = 0; i < len; i++)
        dest[i] = (char)i;
};

const auto schema = bm::create_schema(R"(
{
    "p1":{
        "offset":0,
        "fields":[
            {"name":"p1","length":16,"endian":"big"}
        ]
    },"p2":{
        "offset":16,
        "fields":[
            {"name":"p2","length":16,"endian":"big"}
        ]
    }
}
)");
template<typename M, typename...MapParams>
struct Mapper{
    template<bm::traits::DataProvider Data>
    typename M::ResultsType map(bm::Schema s, Data d, MapParams... params){
        
    }
};

struct ToStringMapper {
    using ResultsType = std::string;
    template<bm::traits::DataProvider Data>
    ResultsType map(bm::Schema s, Data d){
        char buffer[s.calculatedSize()];
        d.read(buffer, s.calculatedSize());
        std::stringstream ss;
        for(auto i = 0; i < s.calculatedSize(); i++)
            ss << "0x" << std::hex << (short)buffer[i] << " ";
        return ss.str();
    }
};
struct RawByteMapper: public Mapper<std::size_t>{
    using ResultsType = std::size_t;
    template<bm::traits::DataProvider Data>
    ResultsType map(bm::Schema s, Data d, std::size_t len){

    }

};
template<typename Mapper, bm::clean::is_schema_type S, bm::traits::DataProvider Data>
bm::Results<typename Mapper::ResultsType> Map(S s, Data d){
    
    return { resutls };
}

struct InlineData{
    InlineData(){
        auto tmp = test_utils::GenerateBinaryData<256>();
    auto write_i = test_utils::WriteStrategy<256>([&](const std::size_t& i, uint8_t& c){
        c = (char)i;
    });
        tmp += write_i;
        memcpy(data, tmp(), 256);
    }
    void read(char* dest, std::size_t n){
        for(auto i = 0; i < n; i++)
            dest[i] = data[i];
    }
    uint8_t data[256];
};
TEST(ConcreteMapperImpl, AbidesByProviderConstraints){
    InlineData data;
    
    const auto results = Map<ToStringMapper>(schema, data);
    std::cout << results.get() << std::endl;
}