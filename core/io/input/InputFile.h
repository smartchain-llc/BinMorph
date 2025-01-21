#pragma once
#include <filesystem>

namespace bm
{

    class InputFile
    {
    public:
        struct Status
        {
            bool is_valid{true};
            std::string msg{""};
        };
        struct DefaultValidator
        {
            static InputFile::Status validate(const std::filesystem::path &p)
            {
                Status ret;
                if (!std::filesystem::exists(p))
                {
                    ret.is_valid = false;
                    ret.msg = "File does not exist";
                return ret;
                }
                if (!std::filesystem::is_regular_file(p))
                {
                    ret.is_valid = false;
                    ret.msg = "File is not regular";
                return ret;
                }
                if (std::filesystem::file_size(p) <= 0)
                {
                    ret.is_valid = false;
                    ret.msg = "File is empty";
                }
                return ret;
            }
        };

    public:
        static std::filesystem::path to_absolute(const std::filesystem::path &relativePath)
        {
            if (relativePath.is_absolute())
                return relativePath;
            return std::filesystem::absolute(relativePath);
        }

        template <typename V = DefaultValidator>
        static InputFile create(const std::initializer_list<std::filesystem::path> filePaths)
        {
            InputFile ret{filePaths};
            const auto status = V::validate(ret.path());
            if (!status.is_valid)
                ret._m_file_status = status;
            return ret;
        }
        template <typename V = DefaultValidator>
        static InputFile create(const std::filesystem::path &filePath)
        {
            InputFile ret{filePath};
            const auto status = V::validate(filePath);
            if (!status.is_valid)
                ret._m_file_status = status;
            return ret;
        }
        [[nodiscard]] inline const std::filesystem::path &path() const noexcept { return _m_path; }
        [[nodiscard]] inline const Status &status() const noexcept { return _m_file_status; }
        
    private:
        InputFile(const std::initializer_list<std::filesystem::path> filePaths)
        {
            std::filesystem::path tmp;
            for (const auto &f : filePaths)
            {
                tmp = tmp / f;
            }
            _m_path = InputFile::to_absolute(tmp);
        }
        InputFile(const std::filesystem::path &filePath) : _m_path(filePath),
                                                           _m_file_status({.is_valid = true})
        {
        }
        std::filesystem::path _m_path;
        Status _m_file_status;
    };
}