#include "support/sample_schema.h"

#include <binmorph/binary_view.h>
#include <binmorph/execution_plan.h>
#include <binmorph/mapped_document.h>
#include <binmorph/mapping_session.h>

#include <gtest/gtest.h>

TEST(ExecutionPlanTest, CompilesFieldStepsFromModel)
{
  const auto model = binmorph::tests::parse_valid_model();

  const auto plan = binmorph::ExecutionPlan::compile(model);

  ASSERT_EQ(plan.steps().size(), 3U);
  EXPECT_EQ(plan.required_size(), 16U);
  EXPECT_EQ(plan.steps()[0].path, "architecture[0].magic");
  EXPECT_EQ(plan.steps()[0].length, 4U);
  EXPECT_EQ(plan.steps()[0].endian, binmorph::Endian::Little);
  EXPECT_EQ(plan.steps()[1].path, "architecture[0].header");
  EXPECT_EQ(plan.steps()[1].endian, binmorph::Endian::Big);
  EXPECT_EQ(plan.steps()[2].path, "architecture[0].payload.metadata");
}

TEST(MappedDocumentTest, StoresAndFindsFieldsByPath)
{
  binmorph::MappedDocument document;
  document.add({
      .path = "root.value",
      .offset = 2,
      .length = 1,
      .endian = binmorph::Endian::Native,
      .raw_bytes = {0x2a},
      .decoded = {42, "42"},
  });

  ASSERT_EQ(document.values().size(), 1U);
  ASSERT_NE(document.find("root.value"), nullptr);
  EXPECT_EQ(document.find("root.value")->decoded.display, "42");
  EXPECT_EQ(document.find("missing"), nullptr);
}

TEST(MappingSessionTest, ParsesBinaryInputIntoMappedDocument)
{
  const auto model = binmorph::tests::parse_valid_model();
  const binmorph::MappingSession session{model};

  const auto result =
      session.parse(binmorph::BinaryView::from_bytes(binmorph::tests::sample_bytes()));

  EXPECT_FALSE(result.diagnostics.has_errors());
  ASSERT_EQ(result.document.values().size(), 3U);

  const auto *magic = result.document.find("architecture[0].magic");
  ASSERT_NE(magic, nullptr);
  EXPECT_EQ(magic->offset, 0U);
  EXPECT_EQ(magic->decoded.unsigned_integer, 1U);

  const auto *header = result.document.find("architecture[0].header");
  ASSERT_NE(header, nullptr);
  EXPECT_EQ(header->decoded.unsigned_integer, 2U);
}

TEST(MappingSessionTest, ReportsShortInput)
{
  const auto model = binmorph::tests::parse_valid_model();
  binmorph::MappingSession session{model};
  session.set_options({.stop_on_error = false});

  const auto result = session.parse(binmorph::BinaryView::from_bytes({0x01, 0x02}));

  EXPECT_TRUE(result.diagnostics.has_errors());
  ASSERT_FALSE(result.diagnostics.entries().empty());
  EXPECT_EQ(result.diagnostics.entries().front().path, "architecture[0].magic");
}

TEST(MappingSessionTest, BuildsMappedDocumentBackToBytes)
{
  const auto model = binmorph::tests::parse_valid_model();
  const binmorph::MappingSession session{model};
  const auto bytes = binmorph::tests::sample_bytes();
  const auto result = session.parse(binmorph::BinaryView::from_bytes(bytes));
  binmorph::Diagnostics diagnostics;

  const auto rebuilt = session.build(result.document, diagnostics);

  EXPECT_FALSE(diagnostics.has_errors());
  EXPECT_EQ(rebuilt, bytes);
}

TEST(MappingSessionTest, ReportsMissingBuildFields)
{
  const auto model = binmorph::tests::parse_valid_model();
  binmorph::MappingSession session{model};
  session.set_options({.stop_on_error = false});
  binmorph::MappedDocument document;
  binmorph::Diagnostics diagnostics;

  const auto rebuilt = session.build(document, diagnostics);

  EXPECT_TRUE(diagnostics.has_errors());
  EXPECT_EQ(rebuilt.size(), session.plan().required_size());
}
