#include <gtest/gtest.h>
#include <TestUtils.h>

#include <bm/schema.h>
#include <bm/io.h>

static auto [data, inschema] = test_utils::Generate256Bytes();

template<bm::traits::Serializable T>
void _serialize(T t){
    bm::Serializer<T> s{ t };
    char buf[2048];
    s.serialize(buf);
}

TEST(SerializerSpecialization, CanSerializeToJSONMapperResutls)
{
    bm::Schema schema = bm::create_schema(inschema);
    bm::BufferedData dataProvider{data, 256};
    const auto results = bm::map_data_against_schema<bm::ToJSONMapper>(schema, dataProvider);
    static_assert(bm::traits::Serializable<bm::ToJSONMapper::ResultsType>);
    _serialize(results);
}









// struct _Test
// {
//     std::size_t s{42069};
//     short ss{1000};
//     uint8_t B{69};
//     bool b{false};
// };
// template <>
// struct bm::Serializer<_Test>
// {
//     public:

//         std::size_t size() const noexcept {
//             return sizeof(_Test);
//         };

//     protected:
//         Serializer(_Test& t): tref{t}{}
//         void serialize(char* dest){

//         }
//         _Test& tref;
// };
// struct TestSerializer : bm::Serializer<_Test>
// {

//     TestSerializer(_Test &test) : bm::Serializer<_Test>(test) {}
//     void serialize(char *dest)
//     {
//         auto inc{0};
//         memcpy(dest, reinterpret_cast<char *>(&tref.s), sizeof(tref.s));
//         inc += sizeof(tref.s);
//         memcpy((dest + inc), reinterpret_cast<char *>(&tref.ss), sizeof(tref.ss));
//         inc += sizeof(tref.ss);
//         memcpy((dest + inc), reinterpret_cast<char *>(&tref.B), sizeof(tref.B));
//         inc += sizeof(tref.B);
//         memcpy((dest + inc), reinterpret_cast<char *>(&tref.b), sizeof(tref.b));
//     }
//     std::size_t size() const noexcept { return sizeof(_Test); }
// };