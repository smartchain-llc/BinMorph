#include <binmorph/mapping_session.h>

#include <algorithm>

namespace binmorph
{

MappingSession::MappingSession(FormatModel model)
    : model_{std::move(model)}, plan_{ExecutionPlan::compile(model_)}
{
}

void MappingSession::set_options(MappingOptions options) noexcept
{
  options_ = options;
}

const FormatModel &MappingSession::model() const noexcept
{
  return model_;
}

const ExecutionPlan &MappingSession::plan() const noexcept
{
  return plan_;
}

CodecRegistry &MappingSession::codecs() noexcept
{
  return codecs_;
}

MappingResult MappingSession::parse(const BinaryView &input) const
{
  MappingResult result;
  BinaryCursor cursor{input};

  for (const auto &step : plan_.steps())
  {
    if (cursor.remaining() < step.length)
    {
      result.diagnostics.error(step.path, cursor.offset(), "not enough bytes remain for field");
      if (options_.stop_on_error)
        break;
      continue;
    }

    const auto offset = cursor.offset();
    const auto bytes = cursor.read(step.length);
    Bytes raw{bytes.begin(), bytes.end()};

    result.document.add({
        .path = step.path,
        .offset = offset,
        .length = step.length,
        .endian = step.endian,
        .raw_bytes = std::move(raw),
        .decoded = codecs_.decode("uint", bytes, step.endian),
    });
  }

  if (cursor.remaining() > 0)
    result.diagnostics.warning(input.source_name(), cursor.offset(), "input has trailing bytes");

  return result;
}

Bytes MappingSession::build(const MappedDocument &document, Diagnostics &diagnostics) const
{
  Bytes output;
  output.reserve(plan_.required_size());

  for (const auto &step : plan_.steps())
  {
    const auto *value = document.find(step.path);
    if (value == nullptr)
    {
      diagnostics.error(step.path, output.size(), "mapped document is missing required field");
      if (options_.stop_on_error)
        break;
      output.resize(output.size() + step.length);
      continue;
    }

    if (value->raw_bytes.size() != step.length)
    {
      diagnostics.error(step.path, output.size(), "field raw byte length does not match schema");
      if (options_.stop_on_error)
        break;
    }

    const auto count = std::min(value->raw_bytes.size(), step.length);
    output.insert(output.end(), value->raw_bytes.begin(), value->raw_bytes.begin() + count);
    if (count < step.length)
      output.resize(output.size() + (step.length - count));
  }

  return output;
}

} // namespace binmorph
