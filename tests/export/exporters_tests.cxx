#include <binmorph/exporters.h>

#include <gtest/gtest.h>

#include <sstream>

namespace
{

binmorph::MappedDocument make_document()
{
  binmorph::MappedDocument document;
  document.add({
      .path = "root.magic",
      .offset = 0,
      .length = 2,
      .endian = binmorph::Endian::Little,
      .raw_bytes = {0x42, 0x4d},
      .decoded = {0x4d42, "19778"},
  });
  return document;
}

} // namespace

TEST(ExporterSuiteTest, ExportsMappedDocumentToJson)
{
  const auto document = make_document();
  const binmorph::ExporterSuite exporters;

  const auto json = exporters.to_json(document);

  ASSERT_TRUE(json.is_array());
  ASSERT_EQ(json.size(), 1U);
  EXPECT_EQ(json.at(0).at("path"), "root.magic");
  EXPECT_EQ(json.at(0).at("offset"), 0U);
  EXPECT_EQ(json.at(0).at("length"), 2U);
  EXPECT_EQ(json.at(0).at("endian"), "little");
  EXPECT_EQ(json.at(0).at("display"), "19778");
  EXPECT_EQ(json.at(0).at("rawBytes").size(), 2U);
}

TEST(ExporterSuiteTest, WritesReadableSummary)
{
  const auto document = make_document();
  binmorph::Diagnostics diagnostics;
  diagnostics.warning("input", 2, "trailing bytes");
  const binmorph::ExporterSuite exporters;
  std::ostringstream output;

  exporters.write_summary(document, diagnostics, output);

  EXPECT_NE(output.str().find("fields: 1"), std::string::npos);
  EXPECT_NE(output.str().find("root.magic @ 0 len 2 = 19778"), std::string::npos);
  EXPECT_NE(output.str().find("diagnostics: 1"), std::string::npos);
  EXPECT_NE(output.str().find("input @ 2: trailing bytes"), std::string::npos);
}
