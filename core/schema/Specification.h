#pragma once
#include <json.hpp>

namespace bm{

/**
### BinMorph JSON: `Specification` Type
A Toplevel Schema JSON object that contains a JSON object

#### Requirements
- Using `template get<T>`, `T` must be *DefaultConstructible*
 */
class Specification {
public:
    Specification(const std::string& key): m_key_{std::move(key)}{}
    void setData(const nlohmann::json& data){
        
    }
    std::string_view key() noexcept { return m_key_; }
private:
    std::string m_key_;
};

}