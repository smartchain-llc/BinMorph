#include <gtest/gtest.h>
#include <bm/traits.h>

#include <TestUtils.h>
using namespace bm::traits;
TEST(IsByteType, AssertsForTypeOfSingleByte){
    static_assert(IsByteType<char>);
    static_assert(!IsByteType<long>);
}
