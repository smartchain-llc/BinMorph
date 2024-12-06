#include <gtest/gtest.h>
#include <TestUtils.h>
#include <json.hpp>
#include <variant>
#include <unordered_map>

static auto SimpleJSON = test_utils::JSONFrom("simple");

struct FieldData{
    std::string name;
    std::size_t length;
    std::string endian;
    std::unordered_map<
        std::string,
        std::variant<std::string, std::size_t, bool>
    > items;
};
template<typename...Fields> struct Data {
    void add(Fields... arg){
        fields.push_back(arg...);
    }
    std::vector<Fields...> fields;
};
void from_json(const nlohmann::json& json, FieldData& fd){
    json["name"].get_to(fd.name);
    json["length"].get_to(fd.length);
    json["endian"].get_to(fd.endian);
}
struct Fields{
    std::vector<FieldData> data;
};
void from_json(const nlohmann::json& json, Fields& f){
    for(auto i = 0; i < json.size(); i++){
        f.data.emplace(f.data.begin(), FieldData{});
        json[i].get_to(f.data[0]);
    }
}
TEST(SimpleJSON, Ihatethis){
    Fields f;
    SimpleJSON.at("fields").get_to(f);
    std::cout << f.data[0].name << std::endl;
}