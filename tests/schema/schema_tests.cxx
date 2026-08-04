#include "support/sample_schema.h"

#include <binmorph/schema_parser.h>
#include <binmorph/schema_registry.h>
#include <binmorph/schema_source.h>
#include <binmorph/schema_validator.h>

#include <gtest/gtest.h>

TEST(SchemaSourceTest, StoresTextAndUriFromString)
{
  const auto source = binmorph::SchemaSource::from_string("{}", "memory://test");

  EXPECT_EQ(source.text(), "{}");
  EXPECT_EQ(source.uri(), "memory://test");
}

TEST(SchemaParserTest, ParsesModelMetadataAndLayout)
{
  binmorph::Diagnostics diagnostics;
  binmorph::SchemaParser parser;

  const auto model = parser.parse(
      binmorph::SchemaSource::from_string(binmorph::tests::sample_schema_text,
                                          "tests/sample.json"),
      diagnostics);

  EXPECT_FALSE(diagnostics.has_errors());
  EXPECT_EQ(model.schema_id(), "tests.sample");
  EXPECT_EQ(model.version(), "1");
  ASSERT_EQ(model.roots().size(), 1U);
  EXPECT_EQ(model.roots().front().kind, binmorph::NodeKind::Repeat);
}

TEST(SchemaParserTest, ReportsMalformedJson)
{
  binmorph::Diagnostics diagnostics;
  binmorph::SchemaParser parser;

  const auto model =
      parser.parse(binmorph::SchemaSource::from_string("{", "bad.json"), diagnostics);

  EXPECT_EQ(model.schema_id(), "bad.json");
  EXPECT_TRUE(diagnostics.has_errors());
}

TEST(SchemaValidatorTest, AcceptsValidSampleSchema)
{
  binmorph::Diagnostics diagnostics;
  binmorph::SchemaParser parser;
  binmorph::SchemaValidator validator;
  const auto model = parser.parse(
      binmorph::SchemaSource::from_string(binmorph::tests::sample_schema_text),
      diagnostics);

  EXPECT_TRUE(validator.validate(model, diagnostics));
  EXPECT_FALSE(diagnostics.has_errors());
}

TEST(SchemaValidatorTest, ReportsInvalidFieldDefinitions)
{
  binmorph::Diagnostics diagnostics;
  binmorph::SchemaParser parser;
  binmorph::SchemaValidator validator;

  const auto model = parser.parse(binmorph::SchemaSource::from_string(R"({
    "bad": { "len": 0, "endian": "sideways" }
  })"),
                                  diagnostics);

  EXPECT_FALSE(validator.validate(model, diagnostics));
  EXPECT_TRUE(diagnostics.has_errors());
}

TEST(SchemaRegistryTest, RegistersAndResolvesModelsBySchemaId)
{
  auto model = binmorph::tests::parse_valid_model();
  binmorph::SchemaRegistry registry;

  registry.register_model(model);

  ASSERT_NE(registry.resolve("tests.sample"), nullptr);
  EXPECT_EQ(registry.resolve("missing"), nullptr);
  EXPECT_EQ(registry.schema_ids(), (std::vector<std::string>{"tests.sample"}));
}
