#include <gtest/gtest.h>
#include <schema/attributes/Partition.h>
static auto json = nlohmann::json::parse(R"(
{
    "magic":{
        "offset": 0,
        "length": 8,
        "fields": [{"name": "header","length": 8, "endian":"big"},
        {"name": "data","length": 4, "endian":"big"}]
        }
}
)");

TEST(Partition, PopulatesAttributes){
    Partition p {json};
    ASSERT_EQ(p.offset, 0);
    ASSERT_EQ(p.key, "magic");
    ASSERT_EQ(p.length, 8);
}
TEST(Partition, ValidatesFieldLengthSumAgainstLengthKey){
    auto _json = nlohmann::json::parse(R"(
    {
    "magic":{
        "offset": 0,
        "length": 16,
        "fields":[
            {"name": "header1","length": 8, "endian":"big"},
            {"name": "header2","length": 8, "endian":"big"},
            {"name": "header3","length": 8, "endian":"big"}
        ]
    }
    }
    )");
    struct Validator{
        bool(*_pred)(const nlohmann::json&);
        Validator(bool(*pred)(const nlohmann::json&)): _pred{pred}{}
        bool operator()(const nlohmann::json& json){
            return _pred(json);
        }
    };
    Validator len{[](const nlohmann::json& j){
        auto attr = j.front();
        auto length = attr["length"];
        auto fields = field_attributes(attr["fields"]);
        unsigned long fieldsLen {0};
        for(const auto& f : fields)
            fieldsLen += f.length;
        return fieldsLen == length;
    }};
    ASSERT_ANY_THROW(Partition::CreateFrom(len, _json));
}