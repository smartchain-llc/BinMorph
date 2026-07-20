#include <cstdint>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "binmorph/binmorph.hpp"

namespace {

using ::testing::_;
using ::testing::HasSubstr;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Throw;

class MockApplicationIO : public binmorph::IApplicationIO {
public:
    MOCK_METHOD(std::string, readSchemaText, (const std::string& path), (override));
    MOCK_METHOD(std::vector<std::uint8_t>, readBinary, (const std::string& path), (override));
    MOCK_METHOD(void,
                writeOutput,
                (const std::string& path, const std::string& payload),
                (override));
};

std::string resourcePath(const std::string& name) {
    return std::string(BINMORPH_TEST_RESOURCES) + "/" + name;
}

std::string readTextResource(const std::string& name) {
    std::ifstream file(resourcePath(name), std::ios::binary);
    if (!file) {
        throw std::runtime_error("missing test resource: " + name);
    }
    std::ostringstream out;
    out << file.rdbuf();
    return out.str();
}

std::vector<std::uint8_t> readHexResource(const std::string& name) {
    std::ifstream file(resourcePath(name));
    if (!file) {
        throw std::runtime_error("missing test resource: " + name);
    }

    std::vector<std::uint8_t> bytes;
    std::string token;
    while (file >> token) {
        bytes.push_back(static_cast<std::uint8_t>(std::stoul(token, nullptr, 16)));
    }
    return bytes;
}

binmorph::NormalizedSchema compileResourceSchema(const std::string& name,
                                                 std::vector<binmorph::Diagnostic>& diagnostics) {
    return binmorph::compileSchema(binmorph::JsonParser(readTextResource(name)).parse(),
                                   diagnostics);
}

bool hasDiagnostic(const std::vector<binmorph::Diagnostic>& diagnostics, const std::string& code) {
    for (const auto& diagnostic : diagnostics) {
        if (diagnostic.code == code) {
            return true;
        }
    }
    return false;
}

} // namespace

TEST(JsonParserTest, ParsesResourceSchemaObject) {
    const binmorph::Json document =
        binmorph::JsonParser(readTextResource("sample-schema.json")).parse();

    ASSERT_EQ(binmorph::JsonType::Object, document.type);
    ASSERT_NE(nullptr, document.get("root"));
    EXPECT_EQ(binmorph::JsonType::Object, document.get("root")->type);
}

TEST(JsonParserTest, RejectsMalformedJsonWithStableExceptionText) {
    EXPECT_THROW(
        {
            try {
                (void)binmorph::JsonParser("{\"root\": [}").parse();
            } catch (const std::runtime_error& error) {
                EXPECT_THAT(error.what(), HasSubstr("JSON parse error"));
                throw;
            }
        },
        std::runtime_error);
}

TEST(SchemaCompilerTest, NormalizesSupportedRootStructFields) {
    std::vector<binmorph::Diagnostic> diagnostics;
    const binmorph::NormalizedSchema schema =
        compileResourceSchema("sample-schema.json", diagnostics);

    EXPECT_TRUE(diagnostics.empty());
    EXPECT_EQ("sample", schema.name);
    ASSERT_EQ(4U, schema.fields.size());
    EXPECT_EQ("magic", schema.fields[0].name);
    EXPECT_EQ(binmorph::FieldKind::Bytes, schema.fields[0].kind);
    EXPECT_EQ(4U, schema.fields[0].size);
    EXPECT_EQ("version", schema.fields[1].name);
    EXPECT_EQ(binmorph::Endian::Little, schema.fields[1].endian);
    EXPECT_EQ("signed_delta", schema.fields[2].name);
    EXPECT_EQ(binmorph::Endian::Big, schema.fields[2].endian);
}

TEST(SchemaCompilerTest, RejectsMultiByteIntegerWithoutExplicitEndianness) {
    std::vector<binmorph::Diagnostic> diagnostics;
    const binmorph::NormalizedSchema schema =
        compileResourceSchema("missing-endian-schema.json", diagnostics);

    EXPECT_TRUE(schema.fields.empty());
    EXPECT_TRUE(hasDiagnostic(diagnostics, "BM_SCHEMA_ENDIAN_REQUIRED"));
}

TEST(SchemaCompilerTest, RejectsUnsupportedFieldTypeAndContinuesValidFields) {
    const std::string schemaText = R"json({
      "dialect": "binmorph.schema.v1",
      "endianness": "little",
      "fields": [
        { "name": "bad", "type": "float32", "offset": 0 },
        { "name": "ok", "type": "u8", "offset": 0 }
      ]
    })json";
    std::vector<binmorph::Diagnostic> diagnostics;

    const binmorph::NormalizedSchema schema =
        binmorph::compileSchema(binmorph::JsonParser(schemaText).parse(), diagnostics);

    ASSERT_EQ(1U, schema.fields.size());
    EXPECT_EQ("ok", schema.fields[0].name);
    EXPECT_TRUE(hasDiagnostic(diagnostics, "BM_SCHEMA_FIELD_TYPE_UNSUPPORTED"));
}

TEST(SchemaCompilerTest, RejectsPolicyLimitForOversizedByteField) {
    const std::string schemaText = R"json({
      "dialect": "binmorph.schema.v1",
      "fields": [
        { "name": "too_big", "type": "bytes", "offset": 0, "size": 67108865 }
      ]
    })json";
    std::vector<binmorph::Diagnostic> diagnostics;

    const binmorph::NormalizedSchema schema =
        binmorph::compileSchema(binmorph::JsonParser(schemaText).parse(), diagnostics);

    EXPECT_TRUE(schema.fields.empty());
    EXPECT_TRUE(hasDiagnostic(diagnostics, "BM_SCHEMA_FIELD_SIZE_LIMIT"));
}

TEST(BinaryViewTest, ReadsValidRangeAndRejectsTruncationAndOverflow) {
    binmorph::BinaryView view({0x01, 0x02, 0x03});
    std::vector<std::uint8_t> out;

    EXPECT_TRUE(view.read(1, 2, out));
    EXPECT_THAT(out, testing::ElementsAre(0x02, 0x03));
    EXPECT_FALSE(view.read(2, 2, out));
    EXPECT_FALSE(view.read(UINT64_MAX, 1, out));
}

TEST(InterpreterTest, DecodesSupportedFieldsFromBinaryFixture) {
    std::vector<binmorph::Diagnostic> diagnostics;
    const binmorph::NormalizedSchema schema =
        compileResourceSchema("sample-schema.json", diagnostics);
    const binmorph::BinaryView view(readHexResource("sample-binary.hex"));

    const binmorph::InterpretResult result =
        binmorph::interpret(schema, view, "fixture", diagnostics);

    EXPECT_TRUE(result.diagnostics.empty());
    ASSERT_EQ(4U, result.fields.size());
    EXPECT_EQ("\"424d5246\"", result.fields[0].valueJson);
    EXPECT_EQ("4660", result.fields[1].valueJson);
    EXPECT_EQ("-2", result.fields[2].valueJson);
    EXPECT_EQ("\"ABC\"", result.fields[3].valueJson);
}

TEST(InterpreterTest, EmitsDiagnosticsForTruncatedInput) {
    std::vector<binmorph::Diagnostic> diagnostics;
    const binmorph::NormalizedSchema schema =
        compileResourceSchema("sample-schema.json", diagnostics);
    const binmorph::BinaryView view(readHexResource("truncated-binary.hex"));

    const binmorph::InterpretResult result =
        binmorph::interpret(schema, view, "fixture", diagnostics);

    EXPECT_TRUE(hasDiagnostic(result.diagnostics, "BM_BINARY_RANGE_UNAVAILABLE"));
    ASSERT_GE(result.fields.size(), 2U);
    EXPECT_FALSE(result.fields[1].ok);
}

TEST(InterpreterTest, EmitsDiagnosticsForNonPrintableAscii) {
    std::vector<binmorph::Diagnostic> diagnostics;
    const binmorph::NormalizedSchema schema =
        compileResourceSchema("sample-schema.json", diagnostics);
    const binmorph::BinaryView view(
        {0x42, 0x4d, 0x52, 0x46, 0x34, 0x12, 0xff, 0xfe, 0x41, 0x01, 0x43});

    const binmorph::InterpretResult result =
        binmorph::interpret(schema, view, "fixture", diagnostics);

    EXPECT_TRUE(hasDiagnostic(result.diagnostics, "BM_BINARY_ASCII_INVALID"));
    ASSERT_EQ(4U, result.fields.size());
    EXPECT_FALSE(result.fields[3].ok);
}

TEST(ApplicationRunnerTest, UsesInjectedIoForSuccessfulInspect) {
    StrictMock<MockApplicationIO> io;
    binmorph::CliOptions options;
    options.schemaPath = "schema";
    options.inputPath = "binary";
    options.outputPath = "out";
    std::string output;

    EXPECT_CALL(io, readSchemaText("schema"))
        .WillOnce(Return(readTextResource("sample-schema.json")));
    EXPECT_CALL(io, readBinary("binary")).WillOnce(Return(readHexResource("sample-binary.hex")));
    EXPECT_CALL(io, writeOutput("out", _))
        .WillOnce([&](const std::string&, const std::string& payload) {
            output = payload;
        });

    EXPECT_EQ(0, binmorph::runInspect(options, io));
    EXPECT_THAT(output, HasSubstr("\"apiVersion\": \"binmorph.inspect.v1\""));
    EXPECT_THAT(output, HasSubstr("\"value\": 4660"));
}

TEST(ApplicationRunnerTest, StopsBeforeBinaryReadWhenSchemaCannotBeRead) {
    StrictMock<MockApplicationIO> io;
    binmorph::CliOptions options;
    options.schemaPath = "schema";
    options.inputPath = "binary";
    options.outputPath = "out";

    EXPECT_CALL(io, readSchemaText("schema"))
        .WillOnce(Throw(std::runtime_error("schema unavailable")));
    EXPECT_CALL(io, writeOutput("out", HasSubstr("BM_SCHEMA_JSON_INVALID")));

    EXPECT_EQ(1, binmorph::runInspect(options, io));
}

TEST(ApplicationRunnerTest, StopsBeforeBinaryReadWhenSchemaJsonIsMalformed) {
    StrictMock<MockApplicationIO> io;
    binmorph::CliOptions options;
    options.schemaPath = "schema";
    options.inputPath = "binary";
    options.outputPath = "out";

    EXPECT_CALL(io, readSchemaText("schema")).WillOnce(Return("{"));
    EXPECT_CALL(io, writeOutput("out", HasSubstr("BM_SCHEMA_JSON_INVALID")));

    EXPECT_EQ(1, binmorph::runInspect(options, io));
}

TEST(ApplicationRunnerTest, EmitsBinarySourceDiagnosticWhenBinaryReadFails) {
    StrictMock<MockApplicationIO> io;
    binmorph::CliOptions options;
    options.schemaPath = "schema";
    options.inputPath = "binary";
    options.outputPath = "out";

    EXPECT_CALL(io, readSchemaText("schema"))
        .WillOnce(Return(readTextResource("sample-schema.json")));
    EXPECT_CALL(io, readBinary("binary")).WillOnce(Throw(std::runtime_error("binary unavailable")));
    EXPECT_CALL(io, writeOutput("out", HasSubstr("BM_BINARY_SOURCE_UNAVAILABLE")));

    EXPECT_EQ(1, binmorph::runInspect(options, io));
}

TEST(ApplicationRunnerTest, HelpReturnsZeroWithoutTouchingIo) {
    StrictMock<MockApplicationIO> io;
    std::ostringstream usage;
    const char* argv[] = {"binmorph", "--help"};

    EXPECT_EQ(0, binmorph::run(2, const_cast<char**>(argv), io, usage));
    EXPECT_THAT(usage.str(), HasSubstr("Usage:"));
}

TEST(ApplicationRunnerTest, MissingCommandPrintsUsageAndThrows) {
    StrictMock<MockApplicationIO> io;
    std::ostringstream usage;
    const char* argv[] = {"binmorph"};

    EXPECT_THROW((void)binmorph::run(1, const_cast<char**>(argv), io, usage), std::runtime_error);
    EXPECT_THAT(usage.str(), HasSubstr("Usage:"));
}
