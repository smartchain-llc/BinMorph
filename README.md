# BinMorph
A C++ Declarative Binary Data Structure Definition Framework

*Declarative Binary Data Structure Definition* refers to a method of defining the layout and interpretation of binary data in a concise and human-readable way, typically using a schema or domain-specific language (DSL). This approach abstracts the complexity of manually parsing binary data and allows developers to focus on describing what the data looks like instead of how to process it.
## Purpose
- Focus on parsing and interpreting existing binary formats.
- Designed for flexibility in working with arbitrary binary data.
- Aims to decode and encoded data in various formats defined by user-provided schemas.
## Concept
Users specify what the binary structure of an object *is*, rather than *how* it's parsed.

This is achieved by using a JSON schema file.

## Use Cases
BinMorph's primary use case is for reverse engineering, analyzing, and working with predefined binary data formats that are not tied to a specific serialization protocol.
#### 1. File Parsing
Use a schema file to interpret binary data of a file.

#### 2. Reverse Engineering
Use a schema file for analyzing unknown binary file formats.

### Output
BinMorph outputs generated C++ source from a processed schema file to provide functionality for parsing and serializing code that matches the schema.

## Alternative Tool Comparisons
### Protobuf
**BinMorph**: Describes and parses any binary data structure, regardless of origin or format

**Protobuf**: Requires a predefined `.proto` schema.

**Summary of Differences**

||||
|---|---|---|
|Feature|	Declarative| Framework|	Protobuf|
|Purpose|	Parsing arbitrary binary formats.|	Efficient serialization for APIs/data.
|Schema|	Flexible, format-agnostic.|	Fixed .proto schema tied to Protobuf.
|Endian Support|	Explicitly handles endianness.|	Abstracted away, no low-level control.
|Field Dependencies|	Supports conditional and dynamic fields.|	Fixed schema, no dynamic dependencies.
|Serialization Format|	Works with arbitrary formats.|	Protobuf-specific format.
|Tooling|	Custom tooling for binary analysis.|	Mature Protobuf ecosystem.

### Data Format Description Language (DFDL)
DFDL is a standardized modeling language designed to describe various data formats, both textual and binary, in a platform-independent manner. It leverages a subset of W3C XML Schema (XSD) to define the logical structure of data and uses annotations to specify its physical representation.

**Summary of Differences**
- Schema Language: JSON (BinMorph) is a more easily digested schema language than XML (DFDL).
The declarative binary data structure project you envision and **Apache DFDL (Data Format Description Language)** share some conceptual similarities in terms of describing and parsing binary data. However, there are fundamental differences in their design philosophies, use cases, and implementation approaches. Here’s an in-depth comparison:



### **Summary of Differences**

| **Feature**                  | **Declarative Binary Data Structure Project**         | **Apache DFDL**                            |
|------------------------------|-----------------------------------------------------|--------------------------------------------|
| **Primary Focus**             | Arbitrary binary data formats.                     | Mixed binary and textual data formats.     |
| **Schema Format**             | JSON/YAML or custom DSL, developer-centric.        | XML-based, W3C XML Schema standard.        |
| **Target Audience**           | Developers, reverse engineers, embedded systems.   | Enterprises, data interoperability.        |
| **Integration**               | Tight language integration (e.g., C++ API).        | Platform-agnostic tools like Apache Daffodil. |
| **Performance**               | High performance, low overhead.                   | Moderate, with emphasis on portability.    |
| **Ease of Use**               | Simpler, developer-friendly.                      | More verbose, requires understanding XML.  |
| **Standardization**           | No adherence to external standards.                | Strict adherence to DFDL and XML Schema.   |

---

### **Differentiating BinMorph**
1. **Developer Focus**:
   - Emphasize ease of use, performance, and tight language integration.
2. **Lightweight Approach**:
   - Provide a simpler schema format (e.g., JSON/YAML) for binary-only use cases.
3. **Advanced Features**:
   - Support bit-level fields, conditional parsing, nested structures, and dynamic lengths.
4. **Open Scope**:
   - Cater to reverse engineering, embedded systems, and proprietary formats.
