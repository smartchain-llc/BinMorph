#pragma once
#include <concepts>
#include <type_traits>
#include <filesystem>

namespace bm{
template<typename T> concept IsPathConvertible = 
    std::convertible_to<T, std::filesystem::path>;

template<typename T> concept HasFilePath = requires {
    requires( requires(T t){ t.filePath() -> IsPathConvertible; } ) || IsPathConvertible<T>;
};

}
// static_assert(IsPathConvertible<const char*>);