#pragma once
#include <concepts>
#include <type_traits>
#include <filesystem>

namespace bm {
namespace traits{
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
    template <typename T>
    concept SchemaProvider = requires {
        requires(
            std::derived_from<Schema, T> || std::convertible_to<Schema, T> ||
            requires(T t) {{ t.schema() } noexcept -> std::same_as<Schema>; }
        );
    };
    template <typename T, typename SP, typename DP>
    concept SchemaMappable = requires {
        requires(SchemaProvider<SP> && DataProvider<DP>);
        requires(
            requires(T t, SP &sp, DP &dp) { {t.map(sp, dp)} -> std::same_as<void>; });
    };
}}