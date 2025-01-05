#pragma once
#include <concepts>
#include <type_traits>
#include <filesystem>

namespace bm {
namespace traits{
    template<typename T>
    concept IsByteType = requires{
        sizeof(T) == 1;
    };
    static_assert(IsByteType<double>);
    
    template <typename T>
    concept IsPathConvertible =
        std::convertible_to<T, std::filesystem::path>;

    template <typename T>
    concept HasFilePath = requires {
        requires(requires(T t) { t.filePath()->IsPathConvertible; }) || IsPathConvertible<T>;
    };
    template <typename T>
    concept DataProvider = requires(T t) {
        { t.length() } noexcept -> std::same_as<std::size_t>;
        requires(requires(char *src, std::size_t len) { t.readTo(src, len); }) ||
                    (requires(uint8_t *src, std::size_t len) { t.readTo(src, len); });
    };

}}