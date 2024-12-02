#include <schema/SchemaFile.h>

using namespace bm;
using namespace nlohmann;

SchemaFile::SchemaFile(std::filesystem::path filePath) : m_path_{std::move(filePath)},
                                                         m_istream_{m_path_}
{
    if(m_istream_){
        m_json_data = json::parse(*this);
    }

}

