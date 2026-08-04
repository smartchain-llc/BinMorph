#pragma once

#include "codec.h"

namespace binmorph
{

class PluginApi
{
public:
  explicit PluginApi(CodecRegistry &codecs);

  void register_codec(std::string name, Codec codec);

private:
  CodecRegistry *codecs_;
};

} // namespace binmorph
