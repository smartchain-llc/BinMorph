#include <binmorph/plugin_api.h>

namespace binmorph
{

PluginApi::PluginApi(CodecRegistry &codecs) : codecs_{&codecs}
{
}

void PluginApi::register_codec(std::string name, Codec codec)
{
  codecs_->register_codec(std::move(name), std::move(codec));
}

} // namespace binmorph
