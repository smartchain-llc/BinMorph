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
struct ToFileWrite{
    ToFileWrite(std::filesystem::path outPath): _outPath{outPath}{}
    std::filesystem::path _outPath;
    template<typename T> void write(T t, std::size_t len){
        std::ofstream outFile { _outPath };
        outFile << t;
    }
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
        
    ToMemWrite mem{32};
    ToFileWrite fout { "/tmp/res" };

    bm::MapAndWrite<bm::ToJSONMapper>(fout, schema, dataToMap);
}
