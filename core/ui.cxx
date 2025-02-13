#include "ui.h"
#include "schema/FieldAttribute.h"

const char* to_json(const char* rawStr){
    std::string toStr{ rawStr };
    std::cout << toStr << std::endl;

    nlohmann::json ret = nlohmann::json::parse(toStr);
    std::string s { ret };
    return s.c_str();
}

static bm_FieldAtt __initalizer;

bm_FieldAtt bm_fa_create(const char* json_cstr){
    nlohmann::json json = nlohmann::json::parse(json_cstr);
    bm::FieldAttribute bmfa = bm::FieldAttribute(json);

    std::cout   << bmfa.name 
                << ", " << bmfa.endian
                << ", " << bmfa.length
                << std::endl;
    bm_FieldAtt ret;
    __initalizer.length = bmfa.length;

    // This does not work because the `string`'s lifetime is local to this function
    __initalizer.name = bmfa.name.c_str();
    __initalizer.endian = bmfa.endian.c_str();

    return __initalizer;
}

bm_FieldAtt* bm_fa_alloc(const char* json_cstr){
    nlohmann::json json = nlohmann::json::parse(json_cstr);
    bm::FieldAttribute bmfa = bm::FieldAttribute(json);

    bm_FieldAtt* ret = new bm_FieldAtt();
    std::string* name = new std::string(bmfa.name);
    std::string* endian = new std::string(bmfa.endian);
    ret->length = bmfa.length;
    ret->name = name->c_str();
    ret->endian = endian->c_str();

    return ret;
}
void bm_fa_free(bm_FieldAtt* fa){
    delete fa->name;
    delete fa->endian;
    delete fa;
}
