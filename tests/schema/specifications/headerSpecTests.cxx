#include <gtest/gtest.h>
#include <schema/Header.h>

#include <TestUtils.h>

#include <unordered_map>

static auto HeaderJSONSchema = test_utils::JSONFrom("header");
using namespace bm;
using namespace nlohmann;

TEST(HeaderSpecification, CanGetToHeaderObjectFromJSON){
    Header hdr;
    HeaderData data;
    HeaderJSONSchema.at(Header::Key).get_to(data);
    ASSERT_EQ(data.headerLength, 32);
}

const std::unordered_map<std::string, Specification> SpecificationMap = {
    {Header::Key, Header{}}
};

struct SchemaParser{
    SchemaParser(const json& inputJSON){
        for(auto& [specification, value]: inputJSON.items()){
            auto spec = SpecificationMap.at(specification);
            spec.setData(value);
        }
    }

    std::vector<Specification> specs;
};
#include <schema/Traits.h>
TEST(HeaderSpecification, AsSpecificationObject){
    Specification header {"header"};
    
}
// TEST(HeaderSpecification, PassesSpecificationTypeTraits){
//     static_assert(is_specification<Header>::value);
//     static_assert(is_specification<decltype(SpecificationMap.at(Header::Key))>::value);
//     auto spec = SpecificationMap.at(Header::Key);
    
//     static_assert(std::is_same_v<SpecificationType<decltype(spec)>, Header>);

//     ASSERT_TRUE(
//         is_specification<decltype(SpecificationMap.at(Header::Key))>::value
//     );
// }

// TEST(HeaderSpecification, CanBeParsedFromParserObject){
//     SchemaParser p{HeaderJSONSchema};
//     decltype(auto) h = SpecificationMap.at(Header::Key);
//     auto isHeaderType = std::is_same_v<Header, decltype(h)>;
//     // ASSERT_EQ(h.length(), 32);
//     ASSERT_TRUE(isHeaderType);
    
// }
