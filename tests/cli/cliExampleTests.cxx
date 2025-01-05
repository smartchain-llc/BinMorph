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
    static_assert(
        bm::traits::HasResultsWriteImpl<CLIWriter, int>
    );
    static_assert(
        bm::clean::is_mappable<bm::mappers::ToJSONMapper, bm::Schema, FileStreamProvider>
    );
    const auto r = bm::Results<int>{69};
}

TEST(CLIUsage, UsingInputSchemaFile){
    const auto inSchemaPath{std::filesystem::path(TEST_SCHEMAS_PATH ) / "256.json"};
    bm::SchemaFile inSchemaFile { inSchemaPath };

    
    FileStreamProvider binFileStream { test_utils::toPath({TEST_DATA_PATH, "256.bin"}) };
    
    const auto results = bm::clean::get_results_of<bm::mappers::ToJSONMapper>(inSchemaFile, binFileStream);
    bm::write_results_to<CLIWriter>(results);
}