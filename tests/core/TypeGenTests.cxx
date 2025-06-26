#include <TestUtils.h>
#include <fstream>
#include <gtest/gtest.h>

#include <schema/Schema.h>
#include <fstream>

#define _GEN_TYPE(NAME) \
    struct NAME         \
    {

#define _TYPE_ADD(ID, LEN) uint8_t ID[LEN];

#define _GEN_END \
    }            \
    ;

template <typename T>
concept is_generatable =
    requires { typename T::generated_type; } &&
    requires(T t) {
        { t.typeStr() } -> std::convertible_to<const std::string>;
        { t.id() } -> std::convertible_to<const std::string>;
    };

class GenTemplate
{
public:
    template <typename T>
        requires is_generatable<T>
    static void Generate(const T &t, std::ostream &out)
    {
        out << t.typeStr();
    }
};

void initHeaderFile(std::filesystem::path &filePath)
{
}
template <typename T>
    requires is_generatable<T>
void generate_stub(const T &t, std::filesystem::path outDir)
{
    if (std::filesystem::exists(outDir))
    {
        outDir /= t.id();
        outDir += std::filesystem::path(T::Id);
        outDir += ".h";

        std::ofstream headerOut(outDir, std::ios::trunc);
        if (!headerOut.good())
        {
            perror(strerror(errno));
            return;
        }
        GenTemplate::Generate(t, headerOut);
        headerOut.flush();
        headerOut.close();
    }
}

template <typename T>
    requires is_generatable<T>
void generate_stub(std::string_view ns, const T &t, std::filesystem::path outDir)
{
    if (std::filesystem::exists(outDir))
    {
        outDir /= t.id();
        outDir += std::filesystem::path(T::Id);
        outDir += ".h";

        std::ofstream headerOut(outDir, std::ios::trunc);
        if (!headerOut.good())
        {
            perror(strerror(errno));
            return;
        }

        headerOut << "#include <cstddef>\n#include <cstdint>\n#pragma once\n"
                  << "namespace " << ns << "{\n";
        GenTemplate::Generate(t, headerOut);

        headerOut << "\n};\n";

        headerOut.flush();
        headerOut.close();
    }
}
class PartitionTypeTemplate
{
public:
    using generated_type = PartitionTypeTemplate;
    static inline constexpr const char *Id = "Partition";

    PartitionTypeTemplate(
        const Partition &from) : _m_from{from} {}
    std::string id() const noexcept { return std::string(_m_from.key()); }
    std::string typeStr() const noexcept
    {
        std::ostringstream oss;
        std::string id{_m_from.key()};
        id[0] = std::toupper(id[0]);

        oss << "struct " << id << "Partition{std::size_t offset {"
            << _m_from.start_offset() << "};\n"
            << "std::size_t length {" << _m_from.length() << "};\n";

        for (const auto field : _m_from.getFields())
        {
            oss << "uint8_t " << field.name << "[" << field.length << "];\n";
        }
        oss << "};\n";
        return oss.str();
    }

private:
    const Partition &_m_from;
};

TEST(TypeGenerator, GeneratesLikeMacros)
{
    _GEN_TYPE(Header)
    _TYPE_ADD(magic, 16)
    _GEN_END

    Header h = {.magic = {0x00}};

    /**
     * Partition: Header
     *      - offset: 0
     *      - fields:
     *          .magic:
     *              = length: 8
     *              = endian: "little"
     */
    const auto schema = Schema::fromJSON(test_utils::JSONFrom("typeGen"));
    for (const auto part : schema)
    {
        PartitionTypeTemplate templ{part};
        generate_stub("typeGen", templ, std::filesystem::path(TEST_PATH));
    }
}