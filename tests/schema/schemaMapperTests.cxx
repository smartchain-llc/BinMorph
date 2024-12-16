#include <gtest/gtest.h>
#include <schema/SchemaMapper.h>
#include <schema/Parser.h>
#include <schema/mappers/ToJSON.h>

#include <TestUtils.h>


TEST(JSONMapper, MapsBinDataAgainstSchemaToJSONResults){
    auto [data, inschema] = test_utils::Generate256Bytes();
    bm::Schema schema;
    bm::Parser::ParseTo(schema, inschema);

    bm::SchemaMapper<bm::ToJSONMapper> mapper;
    mapper.map(schema, data);

    ASSERT_TRUE(!mapper.results()["header"].is_null());
}