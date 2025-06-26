#include "attributes/Partition.h"

struct SchemaResults
{
};
class Schema
{
public:
  static Schema fromJSON(const nlohmann::json &);

public:
  const Partition &getPartition(const std::string &key) const
  {
    auto part = std::find_if(
        _m_parts.begin(),
        _m_parts.end(),
        [&](const Partition &p)
        {
          return p.key() == key;
        });
    if (part != _m_parts.end())
      return *part;
  }
  const Partition &getPartition(const std::size_t &key) const
  {
    if (key < _m_parts.size())
      return _m_parts[key];
  }

  SchemaResults validate(uint8_t *buffer);
  std::vector<Partition>::const_iterator begin() const { return _m_parts.begin(); }
  std::vector<Partition>::const_iterator end() const { return _m_parts.end(); }

protected:
  Schema() {}

private:
  std::vector<Partition> _m_parts;
};
