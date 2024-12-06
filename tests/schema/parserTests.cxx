#include <gtest/gtest.h>
#include <schema/Parser.h>

#include <TestUtils.h>

#include <unordered_map>
#include <functional>
#include <optional>

static auto MultiSpecSchema = test_utils::JSONFrom("multispec");

using namespace bm;

TEST(BinMorphSchemaParser, EnumeratesAllSpecifications)
{
    // const auto first = SchemaParser::GetFirst(MultiSpecSchema);
    // std::cout << first.key << "\n\t" << first.value << std::endl;
    // ASSERT_EQ(first.key, "data");
}
struct Spec{
    Spec(Object result):m_key{result.m_key}{
        auto json = result.m_value[0];
        json.at("length").get_to(length);
        // if(result.m_value.is_object()){
        //     SchemaParser::Print(result.m_value);
        //     SchemaParser::ParseAnd(result.m_value, [&](Object& o){
        //         m_child = std::move(o);
        //     });
        // }
    }
    std::string m_key;
    std::size_t length;
    std::optional<Object> m_child;
};

std::unordered_map<std::string, bool> MappedTypes;
TEST(BinMorphSchemaParser, CanConstructFromObjectsFound){
    std::vector<Object> objs;
    for(const auto& [key, value] : MultiSpecSchema.items()){
        objs.emplace_back(Object{key, value});
    }
    Spec s { objs[0] };
}