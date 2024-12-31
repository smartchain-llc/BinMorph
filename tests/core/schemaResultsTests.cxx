#include <gtest/gtest.h>

#include <TestUtils.h>

#include <bm/schema.h>
#include <bm/io.h>

static auto [data, inschema] = test_utils::Generate256Bytes();
TEST(Results, CanInjectMapperObjectToSchemaMapper)
{
    bm::BufferedData dataProvider{data, 256};
    bm::Schema schema = bm::create_schema(inschema);
    bm::ToJSONMapper toJSON;
    bm::map_data_against_schema(toJSON, schema, dataProvider);

    const auto results = toJSON.results();
}
TEST(Results, CanReturnImplicitMapperResultsTypeObject)
{
    bm::BufferedData dataProvider{data, 256};

    bm::Schema schema = bm::create_schema(inschema);
    const auto results = bm::map_data_against_schema<bm::ToJSONMapper>(schema, dataProvider);
}

template<>
struct bm::Serializer<bm::Results<nlohmann::json>>{
public:

protected:
    Serializer(bm::Results<nlohmann::json>& r): ref{r}{}
    void serialize(char* dest){

    }
    bm::Results<nlohmann::json>& ref;
};
struct RS : bm::Serializer<bm::Results<nlohmann::json>>{
    RS(bm::Results<nlohmann::json> r): bm::Serializer<bm::Results<nlohmann::json>>(r){

    }
    void serialize(char* dest){}
    std::size_t size() const noexcept { return ref.get().size(); }
};

TEST(Results, SatisfiesCanSerializeTraitForSerializableTypes){
    static_assert(bm::traits::Serializable<bm::Results<nlohmann::json>>);
    bm::BufferedData d{data, 256};
    const auto results = bm::SchemaMapper::map<bm::ToJSONMapper>(bm::create_schema(inschema), d);
    
    RS rs{results};
    char dest[rs.size()];
    rs.serialize(dest);
    // std::cout << ss << std::endl;
}
/**********************************************************************
TEST: Results Writer
**********************************************************************/

template <typename Writer, typename ResultsType>
concept CanWriteResults = requires(Writer t, bm::Results<ResultsType> r) {
    requires(requires { t.write(r); }) ||
                (requires { t << r; }) ||
                (requires(const char *s, std::streamsize n) {t.write(s, n); });
};

struct ResultsWriter
{
    template <typename WriterImpl, typename T>
        requires CanWriteResults<WriterImpl, T>
    static void write(const bm::Results<T> &results)
    {
        WriterImpl _w{};
        _w.write(results);
    }
    template <typename WriterImpl, typename T>
        requires CanWriteResults<WriterImpl, T>
    static void write(const WriterImpl &writer, const bm::Results<T> &results)
    {
        writer.write(results);
    }
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
struct FileWriter
{
    FileWriter(std::filesystem::path outPath) : _out{outPath}
    {
        _of = std::fstream{outPath, std::ios::trunc};
        std::string isBad = _of.bad() ? "bad" : "good";
        std::cout << _out << ": " << isBad << '\n';
        if (_of.bad())
            throw "Could not open FileWriter output stream";
    }
    template <typename T>
    void write(const bm::Results<T> &results) const {
        // _of.write()
        // std::cout << results.get() << std::endl;
        // std::cout << "FileWriter ["<<_out<<"]\n";
    } std::filesystem::path _out;
    std::fstream _of;
};

TEST(AbstractResultsWriter, WritesMapperResults)
{
    static_assert(CanWriteResults<CLIWriter, bm::Results<nlohmann::json>>);
    static_assert(CanWriteResults<FileWriter, bm::Results<nlohmann::json>>);

    bm::BufferedData d{data, 256};
    const auto results = bm::SchemaMapper::map<bm::ToJSONMapper>(bm::create_schema(inschema), d);
    static_assert(CanWriteResults<CLIWriter, decltype(results)>);
    static_assert(CanWriteResults<FileWriter, decltype(results)>);
    ResultsWriter::write<CLIWriter>(results);
    // ResultsWriter::write<CLIWriter>(results);
    FileWriter fileout{"/root/test"};
    ResultsWriter::write(fileout, results);
}
TEST(AbstractResultsWriter, CanAcceptstd_fstreamAsArgument)
{
    bm::BufferedData d{data, 256};
    const auto results = bm::SchemaMapper::map<bm::ToJSONMapper>(bm::create_schema(inschema), d);

    static_assert(CanWriteResults<std::fstream, decltype(results)>);
}