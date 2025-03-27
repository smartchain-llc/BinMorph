#pragma once
#include "Segment.h"
#include "FieldAttributes.h"
#include <vector>
struct Partition: public Segment{
    std::string key;
    std::size_t length;
    std::size_t offset;
    std::vector<FieldAttribute> fields;

    Partition(const nlohmann::json& json): 
        Segment([](const nlohmann::json& j){
            
            if(j.find("offset") == j.end()) return false;
            return true;
        }){
            if(!_reqImpl(json.front())) throw "PERR";
            key = json.items().begin().key();
            auto attr = json.front();

            attr["offset"].get_to(offset);
            attr["length"].get_to(length);
            fields = field_attributes(json["fields"]);
    }
};