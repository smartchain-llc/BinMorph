#include <gtest/gtest.h>
#include <bm.h>
#include <TestUtils.h>

struct CoutWrite{
    template<typename T> std::enable_if_t<std::is_constructible_v<T, std::string>, void> write(T t){
        std::cout << t << std::endl;
    }
};
struct ToMemWrite{
    ToMemWrite(std::size_t len): data{ new char[len] }{}
    ~ToMemWrite(){ delete[] data; }

    char* data = nullptr;

    template<typename T> void write(T t, std::size_t len){
        auto l = sizeof(t);
        // for(auto i = 0; i < l; i++)
            // data[i] = *(&t+i);
        memcpy(data, &t, len);
    }
    char* operator()() const { return data; }
};
TEST(MapperResultsOutput, CanOutputAnyWay)
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
    const auto results = bm::Map<bm::ToJSONMapper>(schema, dataToMap, 32);
    static_assert(bm::traits::results_writable<
        bm::ToJSONMapper, 
        CoutWrite,
        decltype(schema),
        decltype(dataToMap),
        decltype(32)>);
    // bm::MapAndWrite<bm::ToJSONMapper, ToMemWrite>(schema, dataToMap, 32);
    ToMemWrite mem{32};
    bm::MapAndWrite<bm::ToJSONMapper>(mem, schema, dataToMap, 32);
    auto memdata = mem();
    for(auto i = 0; i < 32; i+=2){
        if(i % 16 == 0) std::cout << '\n';
        std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << (short)memdata[i] << " ";
    }
    std::cout << '\n';
    std::cout << (memdata) << std::endl;
    bm::MapAndWrite<bm::ToJSONMapper, CoutWrite>(schema, dataToMap, 32);
    // const auto r = Results(&results);
    // std::cout << typeid(r._r).name() << std::endl;
}

// template<> void bm::MapAndWrite<bm::ToJSONMapper, bm::Schema, T, std::size_t>(
//     ToMemWrite& w, bm::Schema& s, T t, std::size_t l
// ){
//     w.write(t, l);
// }
// TEST(MapperResultsOutput, CanBeSpecializedExternally){
//     char dataToMap[32];
//     auto schema = bm::create_schema(R"(
// {
//     "p1": {
//         "offset": 0,
//         "fields":[{"name": "p1.header","length": 8, "endian":"big"}]
//     }
// }
// )");
//     bm::MapAndWrite<bm::ToJSONMapper, ToMemWrite>(schema, dataToMap, 32);

// }