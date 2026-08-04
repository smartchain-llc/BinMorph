#include <binmorph/codec.h>

#include <gtest/gtest.h>

#include <stdexcept>

TEST(CodecTest, DecodesUnsignedIntegersByEndian)
{
  const binmorph::Bytes bytes{0x01, 0x00, 0x00, 0x00};

  const auto little = binmorph::decode_unsigned_integer(bytes, binmorph::Endian::Little);
  ASSERT_TRUE(little.unsigned_integer.has_value());
  EXPECT_EQ(*little.unsigned_integer, 1U);
  EXPECT_EQ(little.display, "1");

  const auto big = binmorph::decode_unsigned_integer(bytes, binmorph::Endian::Big);
  ASSERT_TRUE(big.unsigned_integer.has_value());
  EXPECT_EQ(*big.unsigned_integer, 0x01000000U);
  EXPECT_EQ(big.display, "16777216");
}

TEST(CodecTest, DisplaysLargeValuesAsHexBytes)
{
  const binmorph::Bytes bytes{0x00, 0x01, 0x02, 0x03, 0x04,
                              0x05, 0x06, 0x07, 0x08};

  const auto value = binmorph::decode_unsigned_integer(bytes, binmorph::Endian::Little);

  EXPECT_FALSE(value.unsigned_integer.has_value());
  EXPECT_EQ(value.display, "0x000102030405060708");
}

TEST(CodecRegistryTest, RegistersDefaultAndCustomCodecs)
{
  binmorph::CodecRegistry registry;
  const binmorph::Bytes bytes{0x2a};

  const auto decoded = registry.decode("uint", bytes, binmorph::Endian::Native);
  ASSERT_TRUE(decoded.unsigned_integer.has_value());
  EXPECT_EQ(*decoded.unsigned_integer, 42U);

  registry.register_codec("constant", [](std::span<const binmorph::Byte>, binmorph::Endian)
                          { return binmorph::DecodedValue{7, "seven"}; });
  const auto custom = registry.decode("constant", {}, binmorph::Endian::Native);
  EXPECT_EQ(custom.unsigned_integer, 7U);
  EXPECT_EQ(custom.display, "seven");

  EXPECT_THROW(
      {
        [[maybe_unused]] const auto missing =
            registry.decode("missing", {}, binmorph::Endian::Native);
      },
      std::runtime_error);
}
