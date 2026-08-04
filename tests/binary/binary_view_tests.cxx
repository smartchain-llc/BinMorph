#include <binmorph/binary_view.h>

#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

TEST(BinaryViewTest, StoresBytesAndSourceName)
{
  const auto view = binmorph::BinaryView::from_bytes({0x01, 0x02, 0x03}, "buffer");

  EXPECT_EQ(view.size(), 3U);
  EXPECT_EQ(view.source_name(), "buffer");
  EXPECT_EQ(std::vector<binmorph::Byte>(view.bytes().begin(), view.bytes().end()),
            (binmorph::Bytes{0x01, 0x02, 0x03}));
}

TEST(BinaryViewTest, SlicesBoundsCheckedByteRanges)
{
  const auto view = binmorph::BinaryView::from_bytes({0x01, 0x02, 0x03});
  const auto slice = view.slice(1, 2);

  EXPECT_EQ(std::vector<binmorph::Byte>(slice.begin(), slice.end()),
            (binmorph::Bytes{0x02, 0x03}));
  EXPECT_THROW(
      {
        [[maybe_unused]] const auto out_of_range = view.slice(2, 2);
      },
      std::out_of_range);
}

TEST(BinaryCursorTest, ReadsSequentiallyAndSeeks)
{
  const auto view = binmorph::BinaryView::from_bytes({0x10, 0x20, 0x30});
  binmorph::BinaryCursor cursor{view};

  EXPECT_EQ(cursor.offset(), 0U);
  EXPECT_EQ(cursor.remaining(), 3U);

  const auto first = cursor.read(2);
  EXPECT_EQ(std::vector<binmorph::Byte>(first.begin(), first.end()),
            (binmorph::Bytes{0x10, 0x20}));
  EXPECT_EQ(cursor.offset(), 2U);
  EXPECT_EQ(cursor.remaining(), 1U);

  cursor.seek(1);
  EXPECT_EQ(cursor.offset(), 1U);
  EXPECT_THROW(cursor.seek(4), std::out_of_range);
  EXPECT_THROW(
      {
        [[maybe_unused]] const auto out_of_range = cursor.read(3);
      },
      std::out_of_range);
}
