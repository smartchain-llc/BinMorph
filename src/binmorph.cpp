/**
 * @file binmorph.cpp
 * @brief Implements the current BinMorph schema compiler, interpreter, renderer, and CLI runner.
 */
#include "binmorph/binmorph.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace binmorph {

const Json* Json::get(const std::string& key) const {
    if (type != JsonType::Object) {
        return nullptr;
    }
    const auto it = object.find(key);
    return it == object.end() ? nullptr : &it->second;
}

JsonParser::JsonParser(std::string input)
    : input_(std::move(input)) {}

Json JsonParser::parse() {
    skipWhitespace();
    Json value = parseValue();
    skipWhitespace();
    if (pos_ != input_.size()) {
        fail("unexpected trailing input");
    }
    return value;
}

Json JsonParser::parseValue() {
    skipWhitespace();
    if (pos_ >= input_.size()) {
        fail("unexpected end of input");
    }

    const char c = input_[pos_];
    if (c == '{') {
        return parseObject();
    }
    if (c == '[') {
        return parseArray();
    }
    if (c == '"') {
        Json value;
        value.type = JsonType::String;
        value.string = parseString();
        return value;
    }
    if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
        return parseNumber();
    }
    if (consumeLiteral("true")) {
        Json value;
        value.type = JsonType::Bool;
        value.boolean = true;
        return value;
    }
    if (consumeLiteral("false")) {
        Json value;
        value.type = JsonType::Bool;
        value.boolean = false;
        return value;
    }
    if (consumeLiteral("null")) {
        Json value;
        value.type = JsonType::Null;
        return value;
    }

    fail("unexpected character");
}

Json JsonParser::parseObject() {
    expect('{');
    Json value;
    value.type = JsonType::Object;
    skipWhitespace();
    if (peek('}')) {
        ++pos_;
        return value;
    }

    while (true) {
        skipWhitespace();
        if (!peek('"')) {
            fail("expected object key");
        }
        std::string key = parseString();
        skipWhitespace();
        expect(':');
        value.object.emplace(std::move(key), parseValue());
        skipWhitespace();
        if (peek('}')) {
            ++pos_;
            break;
        }
        expect(',');
    }

    return value;
}

Json JsonParser::parseArray() {
    expect('[');
    Json value;
    value.type = JsonType::Array;
    skipWhitespace();
    if (peek(']')) {
        ++pos_;
        return value;
    }

    while (true) {
        value.array.push_back(parseValue());
        skipWhitespace();
        if (peek(']')) {
            ++pos_;
            break;
        }
        expect(',');
    }

    return value;
}

Json JsonParser::parseNumber() {
    const std::size_t start = pos_;
    if (peek('-')) {
        ++pos_;
    }
    if (pos_ >= input_.size() || !std::isdigit(static_cast<unsigned char>(input_[pos_]))) {
        fail("expected digit");
    }
    if (peek('0')) {
        ++pos_;
    } else {
        while (pos_ < input_.size() && std::isdigit(static_cast<unsigned char>(input_[pos_]))) {
            ++pos_;
        }
    }
    if (pos_ < input_.size() &&
        (input_[pos_] == '.' || input_[pos_] == 'e' || input_[pos_] == 'E')) {
        fail("only integer JSON numbers are supported in schemas");
    }

    Json value;
    value.type = JsonType::Number;
    try {
        value.number = std::stoll(input_.substr(start, pos_ - start));
    } catch (const std::exception&) {
        fail("integer is outside supported range");
    }
    return value;
}

std::string JsonParser::parseString() {
    expect('"');
    std::string out;
    while (pos_ < input_.size()) {
        const char c = input_[pos_++];
        if (c == '"') {
            return out;
        }
        if (static_cast<unsigned char>(c) < 0x20) {
            fail("control character in string");
        }
        if (c != '\\') {
            out.push_back(c);
            continue;
        }

        if (pos_ >= input_.size()) {
            fail("unterminated escape sequence");
        }
        const char escaped = input_[pos_++];
        switch (escaped) {
        case '"':
            out.push_back('"');
            break;
        case '\\':
            out.push_back('\\');
            break;
        case '/':
            out.push_back('/');
            break;
        case 'b':
            out.push_back('\b');
            break;
        case 'f':
            out.push_back('\f');
            break;
        case 'n':
            out.push_back('\n');
            break;
        case 'r':
            out.push_back('\r');
            break;
        case 't':
            out.push_back('\t');
            break;
        case 'u':
            fail("unicode escapes are not supported in this barebones parser");
        default:
            fail("invalid escape sequence");
        }
    }
    fail("unterminated string");
}

bool JsonParser::consumeLiteral(const std::string& literal) {
    if (input_.compare(pos_, literal.size(), literal) != 0) {
        return false;
    }
    pos_ += literal.size();
    return true;
}

void JsonParser::skipWhitespace() {
    while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_]))) {
        ++pos_;
    }
}

bool JsonParser::peek(char c) const {
    return pos_ < input_.size() && input_[pos_] == c;
}

void JsonParser::expect(char c) {
    if (!peek(c)) {
        fail(std::string("expected '") + c + "'");
    }
    ++pos_;
}

void JsonParser::fail(const std::string& message) const {
    std::ostringstream out;
    out << "JSON parse error at byte " << pos_ << ": " << message;
    throw std::runtime_error(out.str());
}

std::string jsonEscape(const std::string& input) {
    std::ostringstream out;
    for (const unsigned char c : input) {
        switch (c) {
        case '"':
            out << "\\\"";
            break;
        case '\\':
            out << "\\\\";
            break;
        case '\b':
            out << "\\b";
            break;
        case '\f':
            out << "\\f";
            break;
        case '\n':
            out << "\\n";
            break;
        case '\r':
            out << "\\r";
            break;
        case '\t':
            out << "\\t";
            break;
        default:
            if (c < 0x20) {
                out << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                    << static_cast<int>(c);
            } else {
                out << static_cast<char>(c);
            }
        }
    }
    return out.str();
}

std::string quote(const std::string& input) {
    return "\"" + jsonEscape(input) + "\"";
}

std::string endianName(Endian endian) {
    switch (endian) {
    case Endian::Little:
        return "little";
    case Endian::Big:
        return "big";
    case Endian::None:
        return "none";
    }
    return "none";
}

Endian parseEndian(const std::string& value) {
    if (value == "little") {
        return Endian::Little;
    }
    if (value == "big") {
        return Endian::Big;
    }
    return Endian::None;
}

bool checkedAdd(std::uint64_t a, std::uint64_t b, std::uint64_t& out) {
    if (a > std::numeric_limits<std::uint64_t>::max() - b) {
        return false;
    }
    out = a + b;
    return true;
}

namespace {

/**
 * @brief Reads a string member from a JSON object.
 *
 * @param[in] object JSON object to inspect.
 * @param[in] key Member name.
 * @param[out] out Receives the string value when present.
 * @retval true The member exists and is a string.
 * @retval false The member is absent or not a string.
 */
bool getString(const Json& object, const std::string& key, std::string& out) {
    const Json* value = object.get(key);
    if (value == nullptr || value->type != JsonType::String) {
        return false;
    }
    out = value->string;
    return true;
}

/**
 * @brief Reads a non-negative integer member from a JSON object.
 *
 * @param[in] object JSON object to inspect.
 * @param[in] key Member name.
 * @param[out] out Receives the converted unsigned value when present.
 * @retval true The member exists, is an integer, and is not negative.
 * @retval false The member is absent, non-numeric, or negative.
 */
bool getUint64(const Json& object, const std::string& key, std::uint64_t& out) {
    const Json* value = object.get(key);
    if (value == nullptr || value->type != JsonType::Number || value->number < 0) {
        return false;
    }
    out = static_cast<std::uint64_t>(value->number);
    return true;
}

/**
 * @brief Reads all bytes from a stream under the binary input policy limit.
 *
 * @param[in,out] input Stream to consume.
 * @return Complete byte payload.
 * @throws std::runtime_error when the payload exceeds @ref kMaxBinaryBytes.
 */
std::vector<std::uint8_t> readBinaryStream(std::istream& input) {
    std::vector<std::uint8_t> bytes;
    char buffer[4096];
    while (input.good()) {
        input.read(buffer, sizeof(buffer));
        const std::streamsize count = input.gcount();
        if (count <= 0) {
            break;
        }
        if (bytes.size() + static_cast<std::size_t>(count) > kMaxBinaryBytes) {
            throw std::runtime_error("binary input exceeds 64 MiB policy limit");
        }
        bytes.insert(bytes.end(), buffer, buffer + count);
    }
    return bytes;
}

} // namespace

void addDiagnostic(std::vector<Diagnostic>& diagnostics,
                   std::string code,
                   std::string message,
                   std::string schemaPath,
                   std::uint64_t binaryOffset,
                   bool hasBinaryOffset) {
    diagnostics.push_back(Diagnostic{std::move(code),
                                     "error",
                                     std::move(message),
                                     std::move(schemaPath),
                                     hasBinaryOffset,
                                     binaryOffset});
}

bool primitiveInfo(const std::string& typeName, FieldKind& kind, std::uint64_t& size) {
    if (typeName == "u8") {
        kind = FieldKind::UnsignedInteger;
        size = 1;
        return true;
    }
    if (typeName == "u16") {
        kind = FieldKind::UnsignedInteger;
        size = 2;
        return true;
    }
    if (typeName == "u32") {
        kind = FieldKind::UnsignedInteger;
        size = 4;
        return true;
    }
    if (typeName == "u64") {
        kind = FieldKind::UnsignedInteger;
        size = 8;
        return true;
    }
    if (typeName == "i8") {
        kind = FieldKind::SignedInteger;
        size = 1;
        return true;
    }
    if (typeName == "i16") {
        kind = FieldKind::SignedInteger;
        size = 2;
        return true;
    }
    if (typeName == "i32") {
        kind = FieldKind::SignedInteger;
        size = 4;
        return true;
    }
    if (typeName == "i64") {
        kind = FieldKind::SignedInteger;
        size = 8;
        return true;
    }
    if (typeName == "bytes") {
        kind = FieldKind::Bytes;
        size = 0;
        return true;
    }
    if (typeName == "ascii") {
        kind = FieldKind::Ascii;
        size = 0;
        return true;
    }
    return false;
}

NormalizedSchema compileSchema(const Json& document, std::vector<Diagnostic>& diagnostics) {
    NormalizedSchema schema;
    if (document.type != JsonType::Object) {
        addDiagnostic(
            diagnostics, "BM_SCHEMA_ROOT_OBJECT", "schema document must be a JSON object", "$");
        return schema;
    }

    std::string value;
    if (getString(document, "name", value)) {
        schema.name = value;
    }
    if (getString(document, "dialect", value)) {
        schema.dialect = value;
        if (schema.dialect != "binmorph.schema.v1") {
            addDiagnostic(diagnostics,
                          "BM_SCHEMA_DIALECT_UNSUPPORTED",
                          "only binmorph.schema.v1 is supported",
                          "$.dialect");
        }
    }
    if (getString(document, "endianness", value)) {
        schema.defaultEndian = parseEndian(value);
        if (schema.defaultEndian == Endian::None) {
            addDiagnostic(diagnostics,
                          "BM_SCHEMA_ENDIAN_INVALID",
                          "endianness must be 'little' or 'big'",
                          "$.endianness");
        }
    }

    const Json* fieldsNode = nullptr;
    std::string fieldsPath = "$.fields";
    if (const Json* root = document.get("root")) {
        if (root->type != JsonType::Object) {
            addDiagnostic(
                diagnostics, "BM_SCHEMA_ROOT_INVALID", "root must be an object", "$.root");
        } else {
            std::string rootType;
            if (getString(*root, "type", rootType) && rootType != "struct") {
                addDiagnostic(diagnostics,
                              "BM_SCHEMA_ROOT_TYPE_UNSUPPORTED",
                              "root.type must be 'struct'",
                              "$.root.type");
            }
            fieldsNode = root->get("fields");
            fieldsPath = "$.root.fields";
        }
    } else {
        fieldsNode = document.get("fields");
    }

    if (fieldsNode == nullptr || fieldsNode->type != JsonType::Array) {
        addDiagnostic(
            diagnostics, "BM_SCHEMA_FIELDS_REQUIRED", "schema requires a fields array", fieldsPath);
        return schema;
    }

    std::uint64_t cursor = 0;
    for (std::size_t i = 0; i < fieldsNode->array.size(); ++i) {
        const Json& fieldJson = fieldsNode->array[i];
        const std::string fieldPath = fieldsPath + "[" + std::to_string(i) + "]";
        if (fieldJson.type != JsonType::Object) {
            addDiagnostic(
                diagnostics, "BM_SCHEMA_FIELD_OBJECT", "field must be an object", fieldPath);
            continue;
        }

        FieldDefinition field;
        field.schemaPath = fieldPath;

        if (!getString(fieldJson, "name", field.name) || field.name.empty()) {
            addDiagnostic(diagnostics,
                          "BM_SCHEMA_FIELD_NAME_REQUIRED",
                          "field.name must be a non-empty string",
                          fieldPath + ".name");
            continue;
        }
        if (!getString(fieldJson, "type", field.typeName) || field.typeName.empty()) {
            addDiagnostic(diagnostics,
                          "BM_SCHEMA_FIELD_TYPE_REQUIRED",
                          "field.type must be a non-empty string",
                          fieldPath + ".type");
            continue;
        }
        if (!primitiveInfo(field.typeName, field.kind, field.size)) {
            addDiagnostic(diagnostics,
                          "BM_SCHEMA_FIELD_TYPE_UNSUPPORTED",
                          "unsupported field type: " + field.typeName,
                          fieldPath + ".type");
            continue;
        }

        std::uint64_t explicitSize = 0;
        if (field.kind == FieldKind::Bytes || field.kind == FieldKind::Ascii) {
            if (!getUint64(fieldJson, "size", explicitSize) || explicitSize == 0) {
                addDiagnostic(diagnostics,
                              "BM_SCHEMA_FIELD_SIZE_REQUIRED",
                              "bytes/ascii fields require a positive size",
                              fieldPath + ".size");
                continue;
            }
            field.size = explicitSize;
        }

        if (!getUint64(fieldJson, "offset", field.offset)) {
            field.offset = cursor;
        }

        field.endian = schema.defaultEndian;
        if (getString(fieldJson, "endian", value)) {
            field.endian = parseEndian(value);
            if (field.endian == Endian::None) {
                addDiagnostic(diagnostics,
                              "BM_SCHEMA_FIELD_ENDIAN_INVALID",
                              "field.endian must be 'little' or 'big'",
                              fieldPath + ".endian");
                continue;
            }
        }
        if ((field.kind == FieldKind::SignedInteger || field.kind == FieldKind::UnsignedInteger) &&
            field.size > 1 && field.endian == Endian::None) {
            addDiagnostic(diagnostics,
                          "BM_SCHEMA_ENDIAN_REQUIRED",
                          "multi-byte integer fields require explicit schema or field endianness",
                          fieldPath + ".endian");
            continue;
        }
        if (field.size > kMaxBinaryBytes) {
            addDiagnostic(diagnostics,
                          "BM_SCHEMA_FIELD_SIZE_LIMIT",
                          "field size exceeds policy limit",
                          fieldPath + ".size");
            continue;
        }

        std::uint64_t next = 0;
        if (!checkedAdd(field.offset, field.size, next)) {
            addDiagnostic(diagnostics,
                          "BM_LAYOUT_RANGE_OVERFLOW",
                          "field offset plus size overflows",
                          fieldPath + ".offset");
            continue;
        }

        cursor = next;
        schema.fields.push_back(std::move(field));
    }

    if (schema.fields.empty() && diagnostics.empty()) {
        addDiagnostic(diagnostics,
                      "BM_SCHEMA_FIELDS_EMPTY",
                      "schema must define at least one field",
                      fieldsPath);
    }

    return schema;
}

BinaryView::BinaryView(std::vector<std::uint8_t> bytes)
    : bytes_(std::move(bytes)) {}

std::size_t BinaryView::size() const {
    return bytes_.size();
}

bool BinaryView::read(std::uint64_t offset,
                      std::uint64_t size,
                      std::vector<std::uint8_t>& out) const {
    std::uint64_t end = 0;
    if (!checkedAdd(offset, size, end)) {
        return false;
    }
    if (end > bytes_.size()) {
        return false;
    }
    const auto begin = bytes_.begin() + static_cast<std::ptrdiff_t>(offset);
    out.assign(begin, begin + static_cast<std::ptrdiff_t>(size));
    return true;
}

/**
 * @brief Converts raw bytes to lowercase hexadecimal text.
 *
 * @param[in] bytes Bytes to encode.
 * @return Two lowercase hex characters per input byte.
 */
std::string hexEncode(const std::vector<std::uint8_t>& bytes) {
    std::ostringstream out;
    out << std::hex << std::setfill('0');
    for (const auto byte : bytes) {
        out << std::setw(2) << static_cast<unsigned int>(byte);
    }
    return out.str();
}

/**
 * @brief Decodes an unsigned integer from one to eight bytes.
 *
 * @param[in] bytes Integer bytes in the requested byte order.
 * @param[in] endian Byte order for multi-byte values.
 * @return Unsigned host integer value.
 * @pre @p bytes contains no more than eight bytes.
 */
std::uint64_t decodeUnsigned(const std::vector<std::uint8_t>& bytes, Endian endian) {
    std::uint64_t value = 0;
    if (endian == Endian::Big) {
        for (const auto byte : bytes) {
            value = (value << 8U) | byte;
        }
        return value;
    }

    for (std::size_t i = 0; i < bytes.size(); ++i) {
        value |= static_cast<std::uint64_t>(bytes[i]) << (i * 8U);
    }
    return value;
}

/**
 * @brief Decodes a signed two's-complement integer from one to eight bytes.
 *
 * @param[in] bytes Integer bytes in the requested byte order.
 * @param[in] endian Byte order for multi-byte values.
 * @return Sign-extended host integer value.
 * @pre @p bytes contains no more than eight bytes.
 */
std::int64_t decodeSigned(const std::vector<std::uint8_t>& bytes, Endian endian) {
    const std::uint64_t unsignedValue = decodeUnsigned(bytes, endian);
    const unsigned bits = static_cast<unsigned>(bytes.size() * 8U);
    if (bits == 64) {
        return static_cast<std::int64_t>(unsignedValue);
    }
    const std::uint64_t signBit = 1ULL << (bits - 1U);
    if ((unsignedValue & signBit) == 0) {
        return static_cast<std::int64_t>(unsignedValue);
    }
    const std::uint64_t extensionMask = ~((1ULL << bits) - 1ULL);
    return static_cast<std::int64_t>(unsignedValue | extensionMask);
}

/**
 * @brief Checks whether every byte is printable ASCII or accepted whitespace.
 *
 * @param[in] bytes Bytes to validate.
 * @retval true All bytes are printable, newline, carriage return, or tab.
 * @retval false At least one byte is outside the accepted ASCII range.
 */
bool isPrintableAscii(const std::vector<std::uint8_t>& bytes) {
    return std::all_of(bytes.begin(), bytes.end(), [](std::uint8_t byte) {
        return byte == '\n' || byte == '\r' || byte == '\t' || (byte >= 0x20 && byte <= 0x7e);
    });
}

InterpretResult interpret(const NormalizedSchema& schema,
                          const BinaryView& view,
                          std::string binarySource,
                          std::vector<Diagnostic> diagnostics) {
    InterpretResult result;
    result.schema = schema;
    result.binarySource = std::move(binarySource);
    result.binarySize = view.size();
    result.diagnostics = std::move(diagnostics);

    for (const FieldDefinition& field : schema.fields) {
        FieldValue fieldValue;
        fieldValue.definition = field;

        std::vector<std::uint8_t> raw;
        if (!view.read(field.offset, field.size, raw)) {
            addDiagnostic(result.diagnostics,
                          "BM_BINARY_RANGE_UNAVAILABLE",
                          "binary input does not contain the requested field range",
                          field.schemaPath,
                          field.offset,
                          true);
            result.fields.push_back(std::move(fieldValue));
            continue;
        }

        fieldValue.ok = true;
        fieldValue.rawHex = hexEncode(raw);
        switch (field.kind) {
        case FieldKind::UnsignedInteger:
            fieldValue.valueJson = std::to_string(decodeUnsigned(raw, field.endian));
            break;
        case FieldKind::SignedInteger:
            fieldValue.valueJson = std::to_string(decodeSigned(raw, field.endian));
            break;
        case FieldKind::Bytes:
            fieldValue.valueJson = quote(fieldValue.rawHex);
            break;
        case FieldKind::Ascii:
            if (!isPrintableAscii(raw)) {
                addDiagnostic(result.diagnostics,
                              "BM_BINARY_ASCII_INVALID",
                              "ascii field contains non-printable bytes",
                              field.schemaPath,
                              field.offset,
                              true);
                fieldValue.ok = false;
            } else {
                fieldValue.valueJson = quote(std::string(raw.begin(), raw.end()));
            }
            break;
        }

        result.fields.push_back(std::move(fieldValue));
    }

    return result;
}

std::string renderDiagnostic(const Diagnostic& diagnostic, int indent) {
    const std::string pad(static_cast<std::size_t>(indent), ' ');
    std::ostringstream out;
    out << pad << "{\n"
        << pad << "  \"code\": " << quote(diagnostic.code) << ",\n"
        << pad << "  \"severity\": " << quote(diagnostic.severity) << ",\n"
        << pad << "  \"message\": " << quote(diagnostic.message) << ",\n"
        << pad << "  \"schemaPath\": " << quote(diagnostic.schemaPath);
    if (diagnostic.hasBinaryOffset) {
        out << ",\n"
            << pad << "  \"binaryOffset\": " << diagnostic.binaryOffset << "\n"
            << pad << "}";
    } else {
        out << "\n" << pad << "}";
    }
    return out.str();
}

std::string renderResult(const InterpretResult& result) {
    std::ostringstream out;
    out << "{\n"
        << "  \"apiVersion\": \"binmorph.inspect.v1\",\n"
        << "  \"schema\": {\n"
        << "    \"name\": " << quote(result.schema.name) << ",\n"
        << "    \"dialect\": " << quote(result.schema.dialect) << ",\n"
        << "    \"endianness\": " << quote(endianName(result.schema.defaultEndian)) << "\n"
        << "  },\n"
        << "  \"binary\": {\n"
        << "    \"source\": " << quote(result.binarySource) << ",\n"
        << "    \"size\": " << result.binarySize << "\n"
        << "  },\n"
        << "  \"root\": {\n"
        << "    \"kind\": \"struct\",\n"
        << "    \"fields\": [\n";

    for (std::size_t i = 0; i < result.fields.size(); ++i) {
        const FieldValue& field = result.fields[i];
        out << "      {\n"
            << "        \"name\": " << quote(field.definition.name) << ",\n"
            << "        \"type\": " << quote(field.definition.typeName) << ",\n"
            << "        \"offset\": " << field.definition.offset << ",\n"
            << "        \"size\": " << field.definition.size << ",\n"
            << "        \"endian\": " << quote(endianName(field.definition.endian)) << ",\n"
            << "        \"status\": " << quote(field.ok ? "ok" : "error");
        if (field.ok) {
            out << ",\n"
                << "        \"rawHex\": " << quote(field.rawHex) << ",\n"
                << "        \"value\": " << field.valueJson << "\n"
                << "      }";
        } else {
            out << "\n"
                << "      }";
        }
        if (i + 1 < result.fields.size()) {
            out << ",";
        }
        out << "\n";
    }

    out << "    ]\n"
        << "  },\n"
        << "  \"diagnostics\": [\n";
    for (std::size_t i = 0; i < result.diagnostics.size(); ++i) {
        out << renderDiagnostic(result.diagnostics[i], 4);
        if (i + 1 < result.diagnostics.size()) {
            out << ",";
        }
        out << "\n";
    }
    out << "  ]\n"
        << "}\n";
    return out.str();
}

std::string renderDiagnosticOnly(std::vector<Diagnostic> diagnostics) {
    InterpretResult result;
    result.diagnostics = std::move(diagnostics);
    return renderResult(result);
}

FilesystemApplicationIO::FilesystemApplicationIO(std::istream& standardInput,
                                                 std::ostream& standardOutput)
    : standardInput_(standardInput)
    , standardOutput_(standardOutput) {}

std::string FilesystemApplicationIO::readSchemaText(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("unable to open schema file: " + path);
    }
    std::ostringstream out;
    out << file.rdbuf();
    std::string text = out.str();
    if (text.size() > kMaxSchemaBytes) {
        throw std::runtime_error("schema input exceeds 1 MiB policy limit");
    }
    return text;
}

std::vector<std::uint8_t> FilesystemApplicationIO::readBinary(const std::string& path) {
    if (path == "-") {
        return readBinaryStream(standardInput_);
    }

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("unable to open binary input: " + path);
    }
    return readBinaryStream(file);
}

void FilesystemApplicationIO::writeOutput(const std::string& path, const std::string& payload) {
    if (path.empty() || path == "-") {
        standardOutput_ << payload;
        return;
    }
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("unable to open output file: " + path);
    }
    file << payload;
}

void printUsage(std::ostream& out) {
    out << "Usage:\n"
        << "  binmorph inspect --schema <schema.json> --input <binary|-> [--output "
           "<out.json|->]\n\n"
        << "Barebones schema dialect:\n"
        << "  "
           "{\"dialect\":\"binmorph.schema.v1\",\"endianness\":\"little\",\"root\":{\"type\":"
           "\"struct\",\"fields\":[..."
           "]}}\n";
}

CliOptions parseCli(int argc, char** argv) {
    if (argc < 2 || std::string(argv[1]) != "inspect") {
        throw std::runtime_error("missing inspect command");
    }

    CliOptions options;
    for (int i = 2; i < argc; ++i) {
        const std::string arg = argv[i];
        auto requireValue = [&](const std::string& flag) -> std::string {
            if (i + 1 >= argc) {
                throw std::runtime_error("missing value for " + flag);
            }
            return argv[++i];
        };

        if (arg == "--schema") {
            options.schemaPath = requireValue(arg);
        } else if (arg == "--input") {
            options.inputPath = requireValue(arg);
        } else if (arg == "--output") {
            options.outputPath = requireValue(arg);
        } else {
            throw std::runtime_error("unknown argument: " + arg);
        }
    }

    if (options.schemaPath.empty()) {
        throw std::runtime_error("--schema is required");
    }
    return options;
}

int runInspect(const CliOptions& options, IApplicationIO& io) {
    std::vector<Diagnostic> diagnostics;
    Json schemaJson;
    try {
        schemaJson = JsonParser(io.readSchemaText(options.schemaPath)).parse();
    } catch (const std::exception& error) {
        addDiagnostic(diagnostics, "BM_SCHEMA_JSON_INVALID", error.what(), "$");
        io.writeOutput(options.outputPath, renderDiagnosticOnly(std::move(diagnostics)));
        return 1;
    }

    NormalizedSchema schema = compileSchema(schemaJson, diagnostics);

    std::vector<std::uint8_t> bytes;
    try {
        bytes = io.readBinary(options.inputPath);
    } catch (const std::exception& error) {
        addDiagnostic(diagnostics, "BM_BINARY_SOURCE_UNAVAILABLE", error.what(), "$");
        InterpretResult result;
        result.schema = schema;
        result.binarySource = options.inputPath == "-" ? "stdin" : options.inputPath;
        result.diagnostics = std::move(diagnostics);
        io.writeOutput(options.outputPath, renderResult(result));
        return 1;
    }

    const BinaryView view(std::move(bytes));
    InterpretResult result = interpret(schema,
                                       view,
                                       options.inputPath == "-" ? "stdin" : options.inputPath,
                                       std::move(diagnostics));
    const bool ok = result.diagnostics.empty();
    io.writeOutput(options.outputPath, renderResult(result));
    return ok ? 0 : 1;
}

int run(int argc, char** argv, IApplicationIO& io, std::ostream& usageOutput) {
    if (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
        printUsage(usageOutput);
        return 0;
    }

    try {
        return runInspect(parseCli(argc, argv), io);
    } catch (const std::exception&) {
        printUsage(usageOutput);
        throw;
    }
}

int runProduction(int argc, char** argv) {
    FilesystemApplicationIO io(std::cin, std::cout);
    return run(argc, argv, io, std::cerr);
}

} // namespace binmorph
