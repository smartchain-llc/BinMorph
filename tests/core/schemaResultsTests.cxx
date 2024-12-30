#include <gtest/gtest.h>

#include <TestUtils.h>

#include <bm/schema.h>
#include <bm/io.h>

static auto [data, inschema] = test_utils::Generate256Bytes();
TEST(Results, CanInjectMapperObjectToSchemaMapper)
{
    bm::BufferedData dataProvider { data, 256 };
    bm::Schema schema = bm::create_schema(inschema);
    bm::ToJSONMapper toJSON;
    bm::map_data_against_schema(toJSON, schema, dataProvider);
    
    const auto results = toJSON.results();
    // ASSERT_TRUE(results.type() != nullptr);
    // std::cout << results << std::endl;
    
}
TEST(Results, CanReturnImplicitMapperResultsTypeObject)
{
    bm::BufferedData dataProvider { data, 256 };

    bm::Schema schema = bm::create_schema(inschema);
    const auto results = bm::map_data_against_schema<bm::ToJSONMapper>(schema, dataProvider);
    // std::cout << results << std::endl;
}

TEST(Results, MostConsiseImpl)
{

    const auto results = bm::map_data_against_schema<bm::ToJSONMapper>(
        bm::create_schema(inschema), 
        bm::BufferedData{data, 256}
    );
    // std::cout << results << std::endl;
}


/**********************************************************************
TEST: Results Writer
**********************************************************************/

template<typename Writer, typename ResultsType>
concept CanWriteResults = requires(Writer t,bm::SchemaMapper::Results<ResultsType> r){
    requires(requires { t.write(r); }) ||
            (requires{ t << r; });
};

struct ResultsWriter{
    template<typename WriterImpl, typename T> requires CanWriteResults<WriterImpl, T>
    static void write(const bm::SchemaMapper::Results<T>& results){
        WriterImpl _w{};
        _w.write(results);
    }
    template<typename WriterImpl, typename T> requires CanWriteResults<WriterImpl, T>
    static void write(const WriterImpl& writer, const bm::SchemaMapper::Results<T>& results){
        writer.write(results);
    }
};
struct CLIWriter{
    template<typename T>
    void write(const bm::SchemaMapper::Results<T>& results){
        std::cout << "CLIWriter [START]\n";
        std::cout << results.results << std::endl;
        std::cout << "CLIWriter [END]\n";
    }
};
struct FileWriter{
    FileWriter(std::filesystem::path outPath): _out{outPath}{}
    template<typename T>
    void write(const bm::SchemaMapper::Results<T>& results) const {
        std::cout << results.results << std::endl;
        std::cout << "FileWriter ["<<_out<<"]\n";
    }
    std::filesystem::path _out;
};

TEST(AbstractResultsWriter, WritesMapperResults){
    static_assert(CanWriteResults<CLIWriter,bm::SchemaMapper::Results<nlohmann::json>>);
    static_assert(CanWriteResults<FileWriter,bm::SchemaMapper::Results<nlohmann::json>>);

    bm::BufferedData d {data, 256};
    const auto results = bm::SchemaMapper::map<bm::ToJSONMapper>(bm::create_schema(inschema), d);
    static_assert(CanWriteResults<CLIWriter,decltype(results)>);
    static_assert(CanWriteResults<FileWriter,decltype(results)>);
    ResultsWriter::write<CLIWriter>(results);
    // ResultsWriter::write<CLIWriter>(results);
    FileWriter fileout{"/tmp/test"};
    ResultsWriter::write(fileout, results);
}