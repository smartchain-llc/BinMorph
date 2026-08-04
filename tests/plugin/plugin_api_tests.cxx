#include <binmorph/plugin_api.h>

#include <gtest/gtest.h>

TEST(PluginApiTest, RegistersCodecsThroughWrappedRegistry)
{
  binmorph::CodecRegistry codecs;
  binmorph::PluginApi plugins{codecs};

  plugins.register_codec("constant", [](std::span<const binmorph::Byte>, binmorph::Endian)
                         { return binmorph::DecodedValue{42, "plugin-value"}; });

  const auto decoded = codecs.decode("constant", {}, binmorph::Endian::Native);
  EXPECT_EQ(decoded.unsigned_integer, 42U);
  EXPECT_EQ(decoded.display, "plugin-value");
}
