#include <gtest/gtest.h>
#include <TestUtils.h>
#include <fsUtils.h>

static auto SimpleJSON = test_utils::JSONFrom("simple");
static auto SCLLC2JSON = test_utils::JSONFrom("scllc2");

TEST(JSON_fsUtils, CanPullJSONFromTestSchemaFileMap){
    ASSERT_TRUE(SimpleJSON != nullptr);
    ASSERT_EQ(SimpleJSON["fields"][0]["name"], "header");
}
/**************************************************
== TESTS: JSON Data Classes
**************************************************/
#include <iostream>
struct JSONData {
    void addField(const std::string& field){
        std::cout << "Added Field: \"" << field << "\"\n";
    }
};
struct Field{
    Field(std::string& key):m_key_{std::move(key)}{}
    std::string m_key_;
};
struct Header {
    std::size_t length { 0 };
};
struct Test{
    std::string name;
    std::size_t length;
};
static void from_json(const nlohmann::json& data, Test& h){
    data["test"].template get<Test>();
}

struct JSONParser{
    static JSONData BuildFrom(nlohmann::json& json){
        JSONData ret;
        Test test;
        const auto scllc_data = json["scllc_data"];
        const auto testData = scllc_data["test"]["name"];
        std::cout << "Hardcoded Test name: " << testData << std::endl;
        // const auto testDataT = scllc_data.template get<Test>();
        Test testDataT {
            scllc_data["test"]["name"].template get<std::string>(),
            scllc_data["test"]["length"].template get<std::size_t>()
        };
        std::cout << "JSON Get Test name: " << testDataT.name << std::endl;
        // from_json(json["scllc_data"], test);
        // std::cout << "Header JSON: " << test.name << std::endl;
        // for(auto it = json.begin(); it != json.end(); ++it){
        //     std::cout << *it << std::endl;
        // }
            
        return ret;
    }
};

TEST(JSONParser, CanBuildFromJSON){
    const auto data = JSONParser::BuildFrom(SCLLC2JSON);
    
}