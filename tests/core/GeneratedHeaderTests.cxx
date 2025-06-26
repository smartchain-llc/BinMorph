#include <generated/headerPartition.h>
#include <TestUtils.h>
#include <fstream>
#include <gtest/gtest.h>

#include <schema/Schema.h>
#include <fstream>

template <typename T>
concept was_generated = requires { typename T; };

TEST(GeneratedType, TypesAreVisible)
{
    static_assert(was_generated<typeGen::HeaderPartition>);
}

TEST(GeneratedType, CanBeMappedWithASchema)
{
    const auto schema = Schema::fromJSON(test_utils::JSONFrom("typeGen"));
    const auto data = test_utils::Generate256Bytes();
    typeGen::HeaderPartition h;
}