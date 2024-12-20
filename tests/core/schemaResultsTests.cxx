#include <gtest/gtest.h>

#include <TestUtils.h>

#include <bm/schema.h>

struct ResultsWriter{
    template<typename T>
    ResultsWriter(const bm::SchemaMapper<T>& m){} // Need to require MapperResults type so m.results() is only needed.
};

TEST(Output, UsesSchemaMapper){

}