#pragma once
#include <json.hpp>
class Segment {
public:
    typedef bool(*RequiredStrategy)(const nlohmann::json&);
protected:
    Segment(RequiredStrategy required): _reqImpl{required}{}
    RequiredStrategy _reqImpl;
};

