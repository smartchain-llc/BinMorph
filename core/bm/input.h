#pragma once

#include "input/InputFile.h"

// namespace bm{

// template<typename T> concept BytePointer = std::same_as<T, char*> || std::same_as<uint8_t*, T>;
// template<typename T> concept DataProvider = requires(T t, std::size_t len) {
//     { t.read(std::declval<char*>(), len) } -> std::same_as<void>;
//     { t.read(std::declval<uint8_t*>(), len) } -> std::same_as<void>;
//     { t.data() } -> std::same_as<uint8_t*>;
// };

// }