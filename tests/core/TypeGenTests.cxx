#include <TestUtils.h>
#include <fstream>
#include <gtest/gtest.h>

#include <schema/Schema.h>

#define _GEN_TYPE(NAME) \
    struct NAME         \
    {

#define _TYPE_ADD(ID, LEN) uint8_t ID[LEN];

#define _GEN_END \
    }            \
    ;

TEST(TypeGenerator, GeneratesLikeMacros)
{
    _GEN_TYPE(Header)
    _TYPE_ADD(magic, 16)
    _GEN_END

    Header h = {.magic = {0x00}};
}