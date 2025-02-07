#include "ui.h"
#include "schema/FieldAttribute.h"

const char* to_json(const char* rawStr){
    std::string toStr{ rawStr };
    std::cout << toStr << std::endl;

    nlohmann::json ret = nlohmann::json::parse(toStr);
    std::string s { ret };
    return s.c_str();
}
void bm_fa_create(const char* json_cstr, bm_FieldAtt* field){
    nlohmann::json json = nlohmann::json::parse(json_cstr);
    std::cout << json << std::endl;
    bm::FieldAttribute bmfa = bm::FieldAttribute(json);

    field->endian = bmfa.endian.c_str();
    field->name = bmfa.name.c_str();
    field->length = bmfa.length;

}