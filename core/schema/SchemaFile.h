// #include <schema/Schema.h>
#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <bm/input.h>
namespace bm
{
    namespace traits{
    template <typename V>
    concept InputSchemaValidator = requires(V v) {
        requires traits::FileValidator<V>;
        { v.getSchema() } -> std::same_as<Schema>;
    };
    }
    /// NOTE: This should only validate BinMorph JSON Schema Syntax, i.e, field layouts, offsets, etc...
    /// it should not be a "JSONProvider".
    /// `FileValidator`'s responsibility is solely to validate files on the filesystem.
    ///
    struct SchemaFileValidator : public bm::DefaultFileValidator
    {
        bm::Schema getSchema(std::filesystem::path filePath) const { 
            std::ifstream in { filePath };
            Schema ret;
            Parser::ParseTo(ret, nlohmann::json::parse(in));
            return ret;
        }
    };

    /// @brief `SchemaFile` is a BinMorph JSON Schema from the filesystem. 
    /// It utilizes the parser to invoke `nlohmann::json` library reader for filestreams then populates a member variable as a `Schema`
    /// `SchemaFile` satifies the concept `traits::SchemaProvider` which requires a `schema()` member function returning the `Schema`.
    class SchemaFile : public InputFile<DefaultFileValidator>
    {
    public:
        SchemaFile(std::filesystem::path filePath): 
            InputFile<DefaultFileValidator>(filePath)
        {
            if(this->isValid())
                _m_schema = _fn_read_file();
        }
        Schema schema() const noexcept {
            return _m_schema;
        }
        operator Schema() const noexcept { return _m_schema; }

    private:
        Schema _fn_read_file() {
            std::ifstream in { this->filePath() };
            Schema ret;
            /// TODO: Implement `SchemaValidator` abstractions to allow for extension of validating return from `Parser` (uses nlohmann:: to parse raw json).
            Parser::ParseTo(ret, nlohmann::json::parse(in));
            return ret;
        }
    private:
        Schema _m_schema;
    };


}