#include <gtest/gtest.h>
#include <schema/Specification.h>

#include <TestUtils.h>
#include <unordered_map>

static auto HeaderJSONSchema = test_utils::JSONFrom("multispec");

using namespace bm;
TEST(SpecificationType, IsConstructedWithJSONKey){
    Specification header {"header"};
    ASSERT_EQ(header.key(), "header");
}

TEST(HeaderSpecification, ExplicitlyDeclaresExpectedDataType){
    // std::vector<Specification> specs;
    // for(const auto& [key, value] : HeaderJSONSchema.items()){
    //     Specification spec { key };
    //     SpecificationData specData;
        

    //     specs.emplace_back(Specification{key});
    // }
    // ASSERT_EQ(specs[0].key(), "header");
    // Maybe have specification specific instantiations have data that derives from
    // required instantiation JSON fields.

    // JSON -> [key, values] -> Header[ "length", "fields"=> [{...}]]
}

// #include <schema/TypeErasedSpec.h>
// std::unordered_map<std::string, SpecificationWrapper> 
// TEST(TypeErasedSpecification, TemplatedConstructorForNonTemplatedClassExample){
//     std::vector<SpecificationWrapper> specs;
//     for(const auto& [key, value] : HeaderJSONSchema.items()){
//         if(key == "header"){
//             HeaderSpec header{};
//             header.fromJSON(value);
//             specs.emplace_back(HeaderSpec{});
//         }
//     }
//     const auto header = specs[0].as<HeaderSpec>();
//     ASSERT_EQ(header.data.len, 32);
// }