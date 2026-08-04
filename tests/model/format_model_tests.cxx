#include <binmorph/format_model.h>

#include <gtest/gtest.h>

TEST(FormatModelTest, StoresSchemaMetadataAndRootNodes)
{
  binmorph::FormatModel model{"schema.id"};
  model.set_version("2");
  model.add_root({
      .name = "root",
      .path = "root",
      .kind = binmorph::NodeKind::Container,
  });

  EXPECT_EQ(model.schema_id(), "schema.id");
  EXPECT_EQ(model.version(), "2");
  EXPECT_FALSE(model.empty());
  ASSERT_EQ(model.roots().size(), 1U);
  EXPECT_EQ(model.roots().front().path, "root");
}

TEST(LayoutNodeTest, IdentifiesFieldNodes)
{
  binmorph::LayoutNode field{.kind = binmorph::NodeKind::Field};
  binmorph::LayoutNode container{.kind = binmorph::NodeKind::Container};

  EXPECT_TRUE(field.is_field());
  EXPECT_FALSE(container.is_field());
}

TEST(FormatModelTest, ConvertsAndParsesEnumText)
{
  EXPECT_EQ(binmorph::to_string(binmorph::Endian::Little), "little");
  EXPECT_EQ(binmorph::to_string(binmorph::Endian::Big), "big");
  EXPECT_EQ(binmorph::to_string(binmorph::Endian::Native), "native");
  EXPECT_EQ(binmorph::to_string(binmorph::NodeKind::Field), "field");
  EXPECT_EQ(binmorph::to_string(binmorph::NodeKind::Repeat), "repeat");

  EXPECT_EQ(binmorph::parse_endian("little"), binmorph::Endian::Little);
  EXPECT_EQ(binmorph::parse_endian("le"), binmorph::Endian::Little);
  EXPECT_EQ(binmorph::parse_endian("big"), binmorph::Endian::Big);
  EXPECT_EQ(binmorph::parse_endian("be"), binmorph::Endian::Big);
  EXPECT_EQ(binmorph::parse_endian("native"), binmorph::Endian::Native);
  EXPECT_FALSE(binmorph::parse_endian("sideways").has_value());
}
