#pragma once
#include <fstream>
#include <functional>
#include <filesystem>
#include <map>

#include <json.hpp>

namespace test_utils
{

    namespace data
    {

        static constexpr std::size_t Simple_Data_Len{10};
        /**
        ### Simple Bin Data
        "header":"uint16_t":"big" = 258
        Data: [ 0x0102 ]
        #### BIG:   `0000 0001   0000 0010` => `0x01 0x02`
        #### LITTLE: `0000 0010 0000 0001` => `0x02 0x01`
         */
        static uint8_t Simple[10] = {
            0x01, 0x02, 0x00, 0xff, 0x01,
            0xfe, 0x00, 0x00, 0xbe, 0xef};

    }
    static constexpr auto CreateEmptyFile = [](const std::filesystem::path &filePath)
    {
        std::ofstream of{filePath, std::ios::trunc};
        of.close();
        return filePath;
    };
    static constexpr auto CreateOpenFile = [](const std::filesystem::path &filePath)
    {
        std::ofstream of{filePath, std::ios::trunc};
        of.close();
        return of;
    };

    enum class CreateOptions
    {
        LEAVE_OPEN
    };

    std::filesystem::path CreateIfNonexistent(const char *name)
    {
        return std::filesystem::exists(name)
                   ? std::filesystem::path{name}
                   : CreateEmptyFile(name);
    }

    static constexpr auto CreateAndWrite = [](const std::filesystem::path &filePath, uint8_t *data, const std::size_t &len)
    {
        auto outfile = CreateEmptyFile(filePath);
        std::ofstream out{outfile};
        uint8_t buffer[len];

        for (auto i = 0; i < len; i++)
            buffer[i] = data[i];
        out.write((char *)buffer, len);
        out.close();
    };

#include <functional>
    template <std::size_t Length>
    struct WriteStrategy
    {
        explicit WriteStrategy(std::function<void(const std::size_t& i, uint8_t& c)> writer): _m_writer{ std::move(writer) }{}
        // const std::size_t writeLength() const noexcept { return Length; }
        void operator()(uint8_t *dest) { 
            for(auto i = 0; i < Length; i++)
                _m_writer(i, dest[i]); 
        }

    private:
        std::function<void(const std::size_t& i, uint8_t& c)> _m_writer;
        std::size_t _m_write_len;
    };

    template <std::size_t Length>
    struct GenerateBinaryData
    {
        uint8_t *operator()() noexcept { return m_data; }
        void set(const std::size_t &from, std::function<void(uint8_t *)> DataSetter)
        {
            DataSetter(&m_data[from]);
        }
        template <std::size_t WriteLength>
        void operator+=(WriteStrategy<WriteLength> &DataWriter)
        {
            if(WriteLength > Length){
                std::cerr << "DataWriter write length (" << WriteLength << ") exceeds buffer size  (" << Length << ")\n";
                return;
            }
            if (WriteLength + _m_write_i > Length)
            {
                _m_write_i = Length - WriteLength;
                std::cerr << "WriteStrategy write length would exceed buffer size\nWriting from byte position: " << _m_write_i << "\n";
            }
            DataWriter(&m_data[_m_write_i]);
            _m_write_i += WriteLength;
        }

    private:
        std::size_t _m_write_i{0};
        uint8_t m_data[Length];
    };
}