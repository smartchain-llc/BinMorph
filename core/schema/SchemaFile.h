// #include <schema/Schema.h>
#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <bm/input.h>
namespace bm
{

    template <typename V>
    concept InputSchemaValidator = requires(V v) {
        requires bm::FileValidator<V>;
        { v.getSchema() } -> std::same_as<bm::Schema>;
    };
    struct SchemaFileValidator : public bm::DefaultFileValidator
    {
        bm::Schema getSchema(std::filesystem::path filePath) const { 
            std::ifstream in { filePath };
            Schema ret;
            Parser::ParseTo(ret, nlohmann::json::parse(in));
            return ret;
        }
    };
    class SchemaFile : public InputFile<SchemaFileValidator>
    {
    public:
        SchemaFile(std::filesystem::path filePath): 
            InputFile<SchemaFileValidator>(filePath),
            _m_schema{ validator().getSchema(filePath) }
        {
        }
        Schema schema() const noexcept {
            return _m_schema;
        }
        operator Schema() const noexcept { return _m_schema; }
    private:
        Schema _m_schema;
    };


}