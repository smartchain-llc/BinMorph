#include <gtest/gtest.h>
#include <bm.h>

TEST(LIB, Works){
    ASSERT_EQ(register_schema("test").id, 0);
}
