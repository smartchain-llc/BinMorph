#pragma once
#include <concepts>
#include <type_traits>
#include <filesystem>

namespace bm {
namespace traits{

    template<typename T>
    concept IsByteType = sizeof(T) == 1;

    template<typename T> struct is_byte_container       : std::false_type { };
    template<typename T> struct is_byte_container<T[]>  : std::enable_if_t<IsByteType<T>, std::true_type>{
        using byte_type = T;
        using qualified_type = T[];
    };
    template<typename T> struct is_byte_container<T*>   : std::enable_if_t<IsByteType<T>, std::true_type>{
        using byte_type = T;
        using qualified_type = T*;
    };;

    template<typename T, std::size_t N> struct is_byte_container<T[N]> : std::enable_if_t<IsByteType<T>, std::true_type>{
        using byte_type = T;
        using qualified_type = T[N];
    };;

    template<typename T> constexpr bool is_byte_container_v = is_byte_container<T>::value;
    
    template<typename T> concept byte_container = is_byte_container_v<T>;

    template <typename T>
    concept IsPathConvertible =
        std::convertible_to<T, std::filesystem::path>;

    template <typename T>
    concept HasFilePath = requires {
        requires(requires(T t) { t.filePath()->IsPathConvertible; }) || IsPathConvertible<T>;
    };

    template <typename ProviderType>
    concept DataProvider = requires(ProviderType t){
    { t.data() } -> byte_container;
    };

    template <typename ProviderType>
    concept DataStreamSelfRet = requires(ProviderType t, char* dest, std::size_t n){
        { t.read(dest, n) } -> std::same_as<std::istream&>;
    };
    template <typename ProviderType>
    concept DataStreamVoidRet = requires(ProviderType t, char* dest, std::size_t n){
        { t.read(dest, n) } -> std::same_as<void>;
    };
    template <typename ProviderType>
    concept DataStream = DataStreamSelfRet<ProviderType> || DataStreamVoidRet<ProviderType>;
};

}
