#pragma once
#include <fstream>
#include <functional>
#include <filesystem>
namespace test_utils{

namespace data{

static constexpr std::size_t Simple_Data_Len { 10 };
/**
### Simple Bin Data
"header":"uint16_t":"big" = 258
Data: [ 0x0102 ]
#### BIG:   `0000 0001   0000 0010` => `0x01 0x02`
#### LITTLE: `0000 0010 0000 0001` => `0x02 0x01`
 */
static uint8_t Simple[10] = {
    0x01, 0x02, 0x00, 0xff, 0x01,
    0xfe, 0x00, 0x00, 0xbe, 0xef
};

}
static constexpr auto CreateEmptyFile = [](const std::filesystem::path& filePath){
  std::ofstream of { filePath, std::ios::trunc};
  of.close();
  return filePath;
};
static constexpr auto CreateOpenFile = [](const std::filesystem::path& filePath){
  std::ofstream of { filePath, std::ios::trunc};
  of.close();
  return of;
};

enum class CreateOptions {
    LEAVE_OPEN
};

std::filesystem::path CreateIfNonexistent(const char* name){
  return std::filesystem::exists(name)
    ? std::filesystem::path{name}
    : CreateEmptyFile(name);
}


static constexpr auto CreateAndWrite = [](const std::filesystem::path& filePath, uint8_t* data, const std::size_t& len){
    auto outfile = CreateEmptyFile(filePath);
    std::ofstream out{outfile};
    uint8_t buffer[len];

    for(auto i = 0; i < len; i++)
        buffer[i] = data[i];
    out.write((char*)buffer, len);
    out.close();
};

}