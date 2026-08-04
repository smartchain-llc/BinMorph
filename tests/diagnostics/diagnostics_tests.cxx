#include <binmorph/diagnostics.h>

#include <gtest/gtest.h>

TEST(DiagnosticsTest, RecordsEntriesInOrder)
{
  binmorph::Diagnostics diagnostics;

  diagnostics.info("root", 0, "loaded");
  diagnostics.warning("root.header", 4, "trailing data");
  diagnostics.error("root.payload", 8, "too short");

  ASSERT_EQ(diagnostics.entries().size(), 3U);
  EXPECT_EQ(diagnostics.entries()[0].severity, binmorph::Severity::Info);
  EXPECT_EQ(diagnostics.entries()[1].severity, binmorph::Severity::Warning);
  EXPECT_EQ(diagnostics.entries()[2].severity, binmorph::Severity::Error);
  EXPECT_EQ(diagnostics.entries()[2].path, "root.payload");
  EXPECT_EQ(diagnostics.entries()[2].offset, 8U);
  EXPECT_EQ(diagnostics.entries()[2].message, "too short");
}

TEST(DiagnosticsTest, HasErrorsOnlyWhenErrorSeverityExists)
{
  binmorph::Diagnostics diagnostics;
  EXPECT_FALSE(diagnostics.has_errors());

  diagnostics.warning("root", 0, "warning");
  EXPECT_FALSE(diagnostics.has_errors());

  diagnostics.error("root", 0, "error");
  EXPECT_TRUE(diagnostics.has_errors());
}
