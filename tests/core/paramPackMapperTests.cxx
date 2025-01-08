#include <gtest/gtest.h>

#include <bm/schema.h>

#include <TestUtils.h>
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

struct MemMapper{
    uint8_t data[1024];
    void map(bm::Schema& s, uint8_t* buf, std::size_t len){
        std::size_t insertOffset { 0 }, readOffset{ 0 };
        for(const auto& p : s){
            memcpy((data + insertOffset), p.id.data(), p.id.length());
            insertOffset += p.id.length();

            memcpy((data + insertOffset), (buf + readOffset), p.byteLength());
            insertOffset += p.byteLength();
            readOffset += p.byteLength();
        }
    }
    bm::Results<uint8_t*> results() { }
};
struct NonDefaultMapper {
    explicit NonDefaultMapper(bool v){}
    void map(bm::Schema s, uint8_t* buf){}
};
TEST(MapperAbstraction, CanBeHomogonouslyStoredInContainer){
    auto write_i = test_utils::WriteStrategy<32>([](const std::size_t& i, uint8_t& c){
        c = (unsigned char)i;
    });
    
    uint8_t buf[32];
    write_i(buf);

    bm::Map<MemMapper>(schema, buf, 32);
    NonDefaultMapper nm{true};
    bm::Map<NonDefaultMapper>(nm, schema, buf);
}