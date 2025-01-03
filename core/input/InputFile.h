#pragma once
#include <filesystem>
#include <bm/traits.h>
namespace bm
{
    namespace traits
    {
        
        template <typename V>
        concept FileValidator = requires(V v, std::filesystem::path f) {
            std::is_same_v<bool, decltype(v.validate(f))>;
            { v.validate(f) };
        };
    }
    /// @brief Default implementation of `FileValidator`
    struct DefaultFileValidator
    {
        bool validate(const std::filesystem::path &filePath)
        {
            if (!std::filesystem::exists(filePath))
                return false;
            if (std::filesystem::is_directory(filePath))
                return false;
            if (!std::filesystem::is_regular_file(filePath))
                return false;
            if (std::filesystem::file_size(filePath) <= 0)
                return false;
            return true;
        }
    };

    /// @brief  Input from file on filesystem 
    /// TODO: Make usage as: `InputFile<V> SchemaFile` OR keep as `SchemaFile: InputFile<V>`?
    /// @tparam V `traits::FileValidator` implementation \see DefaultFileValidator
    template <traits::FileValidator V = DefaultFileValidator>
    class InputFile
    {
    public:
        bool isValid() const noexcept { return _m_isValid; }
        [[nodiscard]] inline const V &validator() const noexcept { return _m_validator; }
        [[nodiscard]] inline const std::filesystem::path &filePath() const noexcept { return _m_path; }
        operator std::filesystem::path() const noexcept { return _m_path; }
        
    protected:
        InputFile(std::filesystem::path filePath) : 
            _m_path{std::move(filePath)}
        {
            _f_init();
        }
        InputFile(std::filesystem::path &&filePath) : 
            _m_path{std::move(filePath)}
        {
            _f_init();
        }

    private:
        void _f_init()
        {
            _m_isValid = _m_validator.validate(_m_path);
        }

    private:
        V _m_validator;
        std::filesystem::path _m_path;
        bool _m_isValid{false};
    };

}