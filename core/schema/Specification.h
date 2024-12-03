#pragma once
#include <json.hpp>

namespace bm{

/**
### BinMorph JSON: `Specification` Type
A JSON object that contains a JSON object

#### Requirements
- Using `template get<T>`, `T` must be *DefaultConstructible*
 */
class Specification {
public:
    Specification(const std::string& key, const nlohmann::json& data):m_key_{std::move(key)}, m_data{data}{}
    std::string_view key() const noexcept { return m_key_; }
    
private:
    nlohmann::json m_data;
    std::string m_key_;
};

}