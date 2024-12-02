#pragma once
#include <json.hpp>

#include <filesystem>
#include <fstream>
#include <future>
#include <thread>

#include <schema/SchemaFileState.h>

namespace bm{


class SchemaFile{
public:
    explicit SchemaFile(std::filesystem::path filePath);
    [[nodiscard]] inline const std::filesystem::path& filePath() const noexcept{ return m_path_; }
    [[nodiscard]] operator std::ifstream&() noexcept { return m_istream_; }
    bool isValid() const noexcept { return state_ == SchemaFileStatus::VALID; }
    const nlohmann::json& json() const noexcept { return m_json_data; }
    
private:
    SchemaFileState state_ { SchemaFileState::InvalidState() };
    // std::future<nlohmann::json> m_json_res_;
    nlohmann::json m_json_data;
    std::filesystem::path m_path_;
    std::ifstream m_istream_;
};

}