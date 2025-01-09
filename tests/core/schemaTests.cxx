#include <gtest/gtest.h>
#include <bm.h>
#include <TestUtils.h>

using namespace bm;
TEST(Schema, CanBeInitializedWithRawStringInput){
    const auto schema = create_schema(R"(
    {
        "p1": {
            "offset": 0
        }
    }
    )");
    static_assert(
        traits::is_schema_validator<DefaultSchemaValidator>
    );
    static_assert(
        std::is_convertible_v<char[], std::string>
    );
    // ASSERT_EQ(schema.get("p1").)
}