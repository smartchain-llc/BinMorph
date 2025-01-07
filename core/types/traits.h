#pragma once
#include <concepts>
#include <type_traits>
#include <filesystem>

namespace bm {
namespace traits{
    template<typename T>
    concept IsByteType = sizeof(T) == 1;
    
    template <typename T>
    concept IsPathConvertible =
        std::convertible_to<T, std::filesystem::path>;

    template <typename T>
    concept HasFilePath = requires {
        requires(requires(T t) { t.filePath()->IsPathConvertible; }) || IsPathConvertible<T>;
    };
    template <typename T>
    concept DataProvider = requires(T t, char* dest, std::size_t n){
    {t.read(dest, n)};
};

}}