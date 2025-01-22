#pragma once
#include <filesystem>

namespace bm
{
namespace file
{
    struct Status
    {
        Status() = default;
        Status(bool valid, const std::string &msg) : is_valid{valid},
                                                        msg{std::move(msg)}
        {
        }
        bool is_valid{true};
        std::string msg{""};
    };
    
namespace traits
{
    template <typename T>
    concept is_file_validator = requires(std::filesystem::path p) {
        { T::validate(p) } -> std::same_as<Status>;
    };
}

    class File
    {
    public:
        [[nodiscard]] inline const std::filesystem::path &path() const noexcept { return _m_path; }
        [[nodiscard]] inline const Status &status() const noexcept { return _m_file_status; }
        friend class FileFactory;

    private:
        File(const std::filesystem::path & fpath, const Status &status):
    _m_path{ std::move(fpath) },
    _m_file_status{ std::move(status) }
{}
        struct DefaultFileValidator
        {
            static file::Status validate(const std::filesystem::path &p)
            {
                file::Status ret;
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

    private:
        std::filesystem::path _m_path;
        Status _m_file_status;
    };

    struct FileFactory
    {
        template <traits::is_file_validator V = File::DefaultFileValidator>
        static File create(const std::filesystem::path &path)
        {
            const auto _absolutePath = path.is_absolute() ? path : std::filesystem::canonical(path);

            const auto _status = V::validate(_absolutePath);
            return File(_absolutePath, _status);
        }
        template <traits::is_file_validator V = File::DefaultFileValidator>
        static File create(const std::initializer_list<std::filesystem::path> pathList)
        {
            std::filesystem::path _path;
            for (const auto &p : pathList)
                _path /= p;
            return create(_path);
        }
    };

}
}