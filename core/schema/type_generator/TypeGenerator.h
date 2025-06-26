#pragma once
#include <filesystem>
#include <map>
#include <fstream>

class TypeTemplate
{
public:
    TypeTemplate(const std::string &id) : _id{std::move(id)} {}

    TypeTemplate &operator<<(std::pair<std::string, std::size_t> &&data)
    {
        _types.insert(data);
    }

protected:
    std::string _id;
    std::map<std::string, std::size_t> _types;
};

class TypeGenerator
{
public:
    static std::filesystem::path typeDirectory;
    static std::ofstream outFile;

    static void Generate(TypeTemplate &&t)
    {
    }
};