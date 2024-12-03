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
    data.at("name").get_to(h.name);
    data.at("length").get_to(h.length);
}
/**
### BinMorph JSON: `Specification` Type
A JSON object that contains a JSON object

#### Requirements
- Using `template get<T>`, `T` must be *DefaultConstructible*
 */
struct Specification {
    Specification() = default;
    Specification(std::string&& key):m_key_{std::move(key)}{}
    Specification(const std::string& key, const nlohmann::json& data):m_key_{std::move(key)}, m_data{data}{
        // m_subspecs_ = JSONParser::GetSpecifications(data);
    }
    std::string_view key() const noexcept { return m_key_; }
    nlohmann::json m_data;
    std::vector<Specification> m_subspecs_;
    std::string m_key_;
};
struct SpecificationParams{
    SpecificationParams() = default;
    SpecificationParams(std::string&& key): m_key{std::move(key)}{}
    SpecificationParams& setKey(std::string& key) noexcept { m_key = key; return *this; }

    std::string m_key;
};
void from_json(const nlohmann::json& data, Specification& s){
    data.at(s.key()).get_to(s.m_data);
}
struct JSONParser{
    static Test GetTestField(const nlohmann::json& data){
        return data.template get<Test>();
    }
    static Specification GetSpecification(const nlohmann::json& data, SpecificationParams& params){
        Specification ret{};
        data.at(params.m_key).get_to(ret.m_data);
        ret.m_key_ = params.m_key;
        return ret;
    }
    static std::vector<Specification> GetSpecifications(const nlohmann::json& data){
        std::vector<Specification> ret;
        for(const auto& item : data.items()){
            if(item.value().is_object())
                ret.push_back({item.key(), item.value()});
        }
        return ret;
    }
    static JSONData BuildFrom(nlohmann::json& json){
        JSONData ret;
        Test test;
        // const auto scllc_data = json["scllc_data"];
        const auto testData = json["scllc_data"]["test"];
        std::cout << "Hardcoded Test name: " << testData << std::endl;
        const auto testDataT = testData.template get<Test>();

        std::cout << "JSON Get Test name: " << testDataT.name << std::endl;

            
        return ret;
    }
};

TEST(JSONParser, CanBuildFromJSON){
    // const auto data = JSONParser::BuildFrom(SCLLC2JSON);
    const auto testField = JSONParser::GetTestField(SCLLC2JSON["scllc_data"]["test"]);
    ASSERT_EQ(testField.length, 420);
}
TEST(JSONParser, CanParseSpecificationJSONTypes){
    SpecificationParams scllcParams {"scllc_data"};
    const auto scllc = JSONParser::GetSpecification(SCLLC2JSON, scllcParams);
    ASSERT_EQ(scllc.key(), "scllc_data");
    ASSERT_EQ(scllc.m_data["test"]["name"], "Test");
}
TEST(JSONParser, CanIterateThroughSPecifications){
    const auto s = JSONParser::GetSpecifications(SCLLC2JSON);
    for(const auto& spec : s)
        std::cout << spec.key() << std::endl;
}