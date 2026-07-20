/**
 * @file binmorph.hpp
 * @brief Declares the current BinMorph schema compiler, interpreter, renderer, and CLI runner.
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

/**
 * @brief Core APIs for the implemented BinMorph CLI and reusable interpretation library.
 */
namespace binmorph {

/** @brief Maximum accepted schema input size in bytes. */
constexpr std::size_t kMaxSchemaBytes = 1024 * 1024;

/** @brief Maximum accepted binary input size in bytes. */
constexpr std::size_t kMaxBinaryBytes = 64 * 1024 * 1024;

/** @brief JSON value category supported by the minimal schema parser. */
enum class JsonType {
    Null,   ///< JSON null.
    Bool,   ///< JSON true or false.
    Number, ///< Signed 64-bit integer JSON number.
    String, ///< UTF-8 byte string after escape processing.
    Array,  ///< Ordered JSON value sequence.
    Object  ///< String-keyed JSON object.
};

/**
 * @brief Minimal JSON value representation used for schema compilation.
 *
 * The active member is selected by @ref type. This representation intentionally supports only
 * integer numbers and does not attempt to preserve source formatting.
 */
struct Json {
    JsonType type = JsonType::Null; ///< Active JSON value category.
    bool boolean = false;           ///< Boolean payload when @ref type is @ref JsonType::Bool.
    std::int64_t number = 0;        ///< Integer payload when @ref type is @ref JsonType::Number.
    std::string string;             ///< String payload when @ref type is @ref JsonType::String.
    std::vector<Json> array;        ///< Array payload when @ref type is @ref JsonType::Array.
    std::map<std::string, Json> object; ///< Object payload when @ref type is @ref JsonType::Object.

    /**
     * @brief Finds a named object member.
     *
     * @param[in] key Object member name.
     * @return Pointer to the stored value, or `nullptr` when this value is not an object or the key
     *         is absent.
     * @ownership The returned pointer is non-owning and remains valid only while this Json object
     * is not mutated or destroyed.
     */
    [[nodiscard]] const Json* get(const std::string& key) const;
};

/**
 * @brief Parses the small JSON subset used by BinMorph schemas.
 *
 * @note Unicode escape sequences and floating-point numbers are rejected because the current schema
 *       dialect does not need them.
 */
class JsonParser {
public:
    /**
     * @brief Stores schema text for parsing.
     *
     * @param[in] input Complete JSON document text.
     */
    explicit JsonParser(std::string input);

    /**
     * @brief Parses the complete input document.
     *
     * @return Parsed JSON value.
     * @throws std::runtime_error when parsing fails or trailing input remains.
     */
    [[nodiscard]] Json parse();

private:
    /**
     * @brief Parses any JSON value at the current cursor.
     *
     * @return Parsed JSON value.
     */
    [[nodiscard]] Json parseValue();

    /**
     * @brief Parses an object value at the current cursor.
     *
     * @return Parsed object value.
     */
    [[nodiscard]] Json parseObject();

    /**
     * @brief Parses an array value at the current cursor.
     *
     * @return Parsed array value.
     */
    [[nodiscard]] Json parseArray();

    /**
     * @brief Parses an integer number at the current cursor.
     *
     * @return Parsed number value.
     */
    [[nodiscard]] Json parseNumber();

    /**
     * @brief Parses a quoted JSON string after validating supported escapes.
     *
     * @return Unescaped string payload.
     */
    [[nodiscard]] std::string parseString();

    /**
     * @brief Consumes a literal at the current cursor when it matches exactly.
     *
     * @param[in] literal Literal text such as `true`, `false`, or `null`.
     * @retval true The literal matched and the cursor advanced.
     * @retval false The literal did not match and the cursor is unchanged.
     */
    bool consumeLiteral(const std::string& literal);

    /** @brief Advances the cursor past JSON whitespace. */
    void skipWhitespace();

    /**
     * @brief Tests the current cursor without advancing it.
     *
     * @param[in] c Character to match.
     * @retval true The current input byte equals @p c.
     * @retval false The cursor is at end of input or the byte differs.
     */
    [[nodiscard]] bool peek(char c) const;

    /**
     * @brief Requires one character at the current cursor.
     *
     * @param[in] c Expected character.
     * @throws std::runtime_error when the expected character is absent.
     */
    void expect(char c);

    /**
     * @brief Raises a parse error annotated with the current byte position.
     *
     * @param[in] message Human-readable parse failure.
     * @throws std::runtime_error always.
     */
    [[noreturn]] void fail(const std::string& message) const;

    std::string input_;   ///< Complete parser input.
    std::size_t pos_ = 0; ///< Zero-based byte cursor into @ref input_.
};

/**
 * @brief Structured validation or interpretation error emitted in JSON output.
 */
struct Diagnostic {
    std::string code;               ///< Stable machine-readable diagnostic identifier.
    std::string severity = "error"; ///< Diagnostic severity; currently always `error`.
    std::string message;            ///< Human-readable diagnostic summary.
    std::string schemaPath;         ///< JSONPath-like location in the schema.
    bool hasBinaryOffset = false;   ///< Whether @ref binaryOffset is meaningful.
    std::uint64_t binaryOffset = 0; ///< Zero-based byte offset related to the diagnostic.
};

/** @brief Byte order used when decoding integer fields. */
enum class Endian {
    None,   ///< No byte order is available or required.
    Little, ///< Least significant byte appears first.
    Big     ///< Most significant byte appears first.
};

/** @brief Normalized primitive field category. */
enum class FieldKind {
    UnsignedInteger, ///< Unsigned fixed-width integer.
    SignedInteger,   ///< Signed fixed-width integer.
    Bytes,           ///< Opaque byte sequence rendered as hexadecimal text.
    Ascii            ///< Printable ASCII byte sequence rendered as JSON text.
};

/**
 * @brief Validated field layout entry in a normalized schema.
 */
struct FieldDefinition {
    std::string name;                  ///< Field name from the schema.
    std::string typeName;              ///< Source schema type token.
    FieldKind kind = FieldKind::Bytes; ///< Normalized primitive category.
    std::uint64_t offset = 0;          ///< Zero-based byte offset in the input binary.
    std::uint64_t size = 0;            ///< Field width in bytes.
    Endian endian = Endian::None;      ///< Byte order used for integer fields.
    std::string schemaPath;            ///< JSONPath-like field location in the schema.
};

/**
 * @brief Minimal compiled representation of the current schema dialect.
 *
 * @invariant Every field has a supported primitive type and a checked offset plus size range.
 */
struct NormalizedSchema {
    std::string name = "unnamed";               ///< Schema name used in rendered output.
    std::string dialect = "binmorph.schema.v1"; ///< Schema dialect identifier.
    Endian defaultEndian = Endian::None;        ///< Schema-wide default integer byte order.
    std::vector<FieldDefinition> fields;        ///< Root struct fields in decode order.
};

/**
 * @brief Decoded value and raw bytes for one normalized field.
 */
struct FieldValue {
    FieldDefinition definition; ///< Field layout metadata copied from the schema.
    bool ok = false;            ///< Whether field bytes were decoded without a diagnostic.
    std::string valueJson;      ///< JSON fragment for the decoded value when @ref ok is true.
    std::string rawHex;         ///< Hexadecimal encoding of the raw field bytes.
};

/**
 * @brief Complete result model rendered as `binmorph.inspect.v1` JSON.
 */
struct InterpretResult {
    NormalizedSchema schema;             ///< Schema metadata used for interpretation.
    std::string binarySource;            ///< File path or `stdin` label for the binary input.
    std::size_t binarySize = 0;          ///< Total binary input size in bytes.
    std::vector<FieldValue> fields;      ///< Decoded root fields.
    std::vector<Diagnostic> diagnostics; ///< Diagnostics accumulated across the workflow.
};

/**
 * @brief Bounded byte container used by the interpreter.
 *
 * @ownership Owns the bytes supplied at construction.
 */
class BinaryView {
public:
    /**
     * @brief Takes ownership of binary input bytes.
     *
     * @param[in] bytes Complete binary payload.
     */
    explicit BinaryView(std::vector<std::uint8_t> bytes);

    /**
     * @brief Returns the total byte length of the view.
     *
     * @return Number of bytes available for checked reads.
     */
    [[nodiscard]] std::size_t size() const;

    /**
     * @brief Copies a checked byte range from the view.
     *
     * @param[in] offset Zero-based byte offset.
     * @param[in] size Number of bytes to copy.
     * @param[out] out Receives the copied bytes on success.
     * @retval true The requested range was within the view.
     * @retval false The requested range overflowed or extended past the view.
     */
    bool read(std::uint64_t offset, std::uint64_t size, std::vector<std::uint8_t>& out) const;

private:
    std::vector<std::uint8_t> bytes_; ///< Owned binary payload.
};

/**
 * @brief Parsed options for the `inspect` command.
 */
struct CliOptions {
    std::string schemaPath;       ///< Required schema JSON file path.
    std::string inputPath = "-";  ///< Binary input path, or `-` for stdin.
    std::string outputPath = "-"; ///< Output JSON path, or `-` for stdout.
};

/**
 * @brief IO port used to keep CLI orchestration testable.
 */
class IApplicationIO {
public:
    virtual ~IApplicationIO() = default;

    /**
     * @brief Reads schema text.
     *
     * @param[in] path Schema path supplied by the CLI.
     * @return Complete schema document text.
     * @throws std::runtime_error when the schema cannot be read.
     */
    [[nodiscard]] virtual std::string readSchemaText(const std::string& path) = 0;

    /**
     * @brief Reads binary input bytes.
     *
     * @param[in] path Binary path, or `-` for stdin.
     * @return Binary input bytes.
     * @throws std::runtime_error when input cannot be read or exceeds policy limits.
     */
    [[nodiscard]] virtual std::vector<std::uint8_t> readBinary(const std::string& path) = 0;

    /**
     * @brief Writes rendered JSON output.
     *
     * @param[in] path Output path, empty string, or `-` for stdout.
     * @param[in] payload Complete JSON payload.
     * @throws std::runtime_error when file output cannot be opened.
     */
    virtual void writeOutput(const std::string& path, const std::string& payload) = 0;
};

/**
 * @brief Production IO adapter for files, stdin, and stdout.
 *
 * @ownership Stores non-owning references to the standard input and output streams supplied at
 *            construction.
 */
class FilesystemApplicationIO final : public IApplicationIO {
public:
    /**
     * @brief Binds the adapter to standard streams.
     *
     * @param[in,out] standardInput Stream used when a binary input path is `-`.
     * @param[in,out] standardOutput Stream used when output path is empty or `-`.
     */
    explicit FilesystemApplicationIO(std::istream& standardInput, std::ostream& standardOutput);

    [[nodiscard]] std::string readSchemaText(const std::string& path) override;
    [[nodiscard]] std::vector<std::uint8_t> readBinary(const std::string& path) override;
    void writeOutput(const std::string& path, const std::string& payload) override;

private:
    std::istream& standardInput_;  ///< Non-owning standard input stream reference.
    std::ostream& standardOutput_; ///< Non-owning standard output stream reference.
};

/**
 * @brief Escapes text for inclusion in a JSON string body.
 *
 * @param[in] input Unescaped text.
 * @return Escaped text without surrounding quotation marks.
 */
[[nodiscard]] std::string jsonEscape(const std::string& input);

/**
 * @brief Escapes and quotes text as a JSON string value.
 *
 * @param[in] input Unescaped text.
 * @return Complete JSON string value.
 */
[[nodiscard]] std::string quote(const std::string& input);

/**
 * @brief Converts a byte-order value into its schema spelling.
 *
 * @param[in] endian Byte order value.
 * @return `little`, `big`, or `none`.
 */
[[nodiscard]] std::string endianName(Endian endian);

/**
 * @brief Parses a schema byte-order token.
 *
 * @param[in] value Schema token.
 * @return Parsed byte order, or @ref Endian::None for an unsupported token.
 */
[[nodiscard]] Endian parseEndian(const std::string& value);

/**
 * @brief Adds two unsigned byte counts while detecting overflow.
 *
 * @param[in] a First addend.
 * @param[in] b Second addend.
 * @param[out] out Sum when the operation succeeds.
 * @retval true The sum fit in `std::uint64_t`.
 * @retval false The addition would overflow.
 */
bool checkedAdd(std::uint64_t a, std::uint64_t b, std::uint64_t& out);

/**
 * @brief Appends an error diagnostic.
 *
 * @param[in,out] diagnostics Destination diagnostic list.
 * @param[in] code Stable diagnostic identifier.
 * @param[in] message Human-readable diagnostic message.
 * @param[in] schemaPath JSONPath-like schema location.
 * @param[in] binaryOffset Related binary byte offset when present.
 * @param[in] hasBinaryOffset Whether @p binaryOffset is meaningful.
 */
void addDiagnostic(std::vector<Diagnostic>& diagnostics,
                   std::string code,
                   std::string message,
                   std::string schemaPath,
                   std::uint64_t binaryOffset = 0,
                   bool hasBinaryOffset = false);

/**
 * @brief Looks up primitive type metadata for the current schema dialect.
 *
 * @param[in] typeName Schema type token.
 * @param[out] kind Normalized field category.
 * @param[out] size Fixed type width in bytes, or zero for variable-sized tokens.
 * @retval true The type token is supported.
 * @retval false The type token is not supported.
 */
bool primitiveInfo(const std::string& typeName, FieldKind& kind, std::uint64_t& size);

/**
 * @brief Compiles a parsed schema document into the minimal normalized model.
 *
 * @param[in] document Parsed schema JSON.
 * @param[in,out] diagnostics Receives schema validation diagnostics.
 * @return Normalized schema containing every valid field that can be compiled.
 */
[[nodiscard]] NormalizedSchema compileSchema(const Json& document,
                                             std::vector<Diagnostic>& diagnostics);

/**
 * @brief Decodes normalized fields from a bounded binary view.
 *
 * @param[in] schema Normalized schema to interpret.
 * @param[in] view Bounded binary bytes.
 * @param[in] binarySource Source label rendered into the result.
 * @param[in] diagnostics Diagnostics already produced by earlier workflow steps.
 * @return Complete interpretation result with decoded fields and diagnostics.
 */
[[nodiscard]] InterpretResult interpret(const NormalizedSchema& schema,
                                        const BinaryView& view,
                                        std::string binarySource,
                                        std::vector<Diagnostic> diagnostics);

/**
 * @brief Renders one diagnostic as indented JSON.
 *
 * @param[in] diagnostic Diagnostic to render.
 * @param[in] indent Number of leading spaces for the opening brace.
 * @return JSON object fragment.
 */
[[nodiscard]] std::string renderDiagnostic(const Diagnostic& diagnostic, int indent);

/**
 * @brief Renders a complete interpretation result as `binmorph.inspect.v1` JSON.
 *
 * @param[in] result Interpretation result model.
 * @return Complete JSON document with a trailing newline.
 */
[[nodiscard]] std::string renderResult(const InterpretResult& result);

/**
 * @brief Renders diagnostics without decoded binary fields.
 *
 * @param[in] diagnostics Diagnostics to place in a result document.
 * @return Complete JSON document with a trailing newline.
 */
[[nodiscard]] std::string renderDiagnosticOnly(std::vector<Diagnostic> diagnostics);

/**
 * @brief Writes command usage text.
 *
 * @param[in,out] out Destination stream.
 */
void printUsage(std::ostream& out);

/**
 * @brief Parses command-line arguments for the implemented CLI.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 * @return Parsed inspect options.
 * @throws std::runtime_error when the command or required options are invalid.
 */
[[nodiscard]] CliOptions parseCli(int argc, char** argv);

/**
 * @brief Runs the inspect workflow with injected IO.
 *
 * @param[in] options Parsed inspect options.
 * @param[in,out] io IO adapter used for schema input, binary input, and output.
 * @retval 0 No diagnostics were emitted.
 * @retval 1 The schema, binary input, or interpretation produced diagnostics.
 */
int runInspect(const CliOptions& options, IApplicationIO& io);

/**
 * @brief Runs the CLI with injected IO and usage output.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 * @param[in,out] io IO adapter used by the inspect command.
 * @param[in,out] usageOutput Stream receiving usage text for help or argument failures.
 * @return Process-style exit code.
 * @throws std::runtime_error when arguments are invalid after usage has been printed.
 */
int run(int argc, char** argv, IApplicationIO& io, std::ostream& usageOutput);

/**
 * @brief Production composition root for the `binmorph` executable.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 * @return Process-style exit code.
 */
int runProduction(int argc, char** argv);

} // namespace binmorph
