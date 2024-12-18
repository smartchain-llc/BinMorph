// #include <schema/Schema.h>
#include <filesystem>
#include <json.hpp>
#include <fstream>
namespace bm{

    class SchemaFile{
    public:
        SchemaFile(std::filesystem::path filePath){
            // validate file path
            std::ifstream in { filePath };
            _m_json = nlohmann::json::parse(in);
        }
        operator nlohmann::json() const noexcept { return _m_json; }
        const nlohmann::json& JSON() const noexcept { return _m_json; }
    private:
        nlohmann::json _m_json;
    };

}