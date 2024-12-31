#include <gtest/gtest.h>

#include <bm/schema.h>

#include <TestUtils.h>


/**********************************************************************
SETUP: Abstraction Implementaters Definitions
**********************************************************************/
struct FileStreamProvider{
    FileStreamProvider(std::filesystem::path p): _path{p}{}
    void readTo(uint8_t* dest, std::size_t len){
        std::ifstream in {_path};
        in.read((char*)dest, len);
    }
    std::size_t length() noexcept { return std::filesystem::file_size(_path); }
    std::filesystem::path _path;
};
struct CLIWriter
{
    template <typename T>
    void write(const bm::Results<T> &results)
    {
        std::cout << "CLIWriter [START]\n";
        std::cout << results.get() << std::endl;
        std::cout << "CLIWriter [END]\n";
    }
};

/**********************************************************************
TEST: 
**********************************************************************/
TEST(TraitsSanityCheck, AssertsAllNeededTraits){
    static_assert(bm::traits::InputJSONProvider<bm::SchemaFile>);
    static_assert(std::is_convertible_v<bm::SchemaFile, nlohmann::json>);
    static_assert(bm::CanWriteResults<CLIWriter, bm::Results<bm::mappers::ToJSONMapper::ResultsType>>);
}

TEST(CLIUsage, UsingInputSchemaFile){
    const auto inSchemaPath{std::filesystem::path(TEST_SCHEMAS_PATH ) / "256.json"};
    bm::SchemaFile inSchemaFile { inSchemaPath };

    const auto schema = bm::create_schema_t( inSchemaFile );
    
    FileStreamProvider binFileStream { test_utils::toPath({TEST_DATA_PATH, "256.bin"}) };

    const auto results = bm::map_data_against_schema<bm::mappers::ToJSONMapper>(schema, binFileStream);
    bm::ResultsWriter::write<CLIWriter>(results); 
    // bm::ResultsWriter::write<CLIWriter, bm::mappers::ToJSONMapper::ResultsType>(results);
}