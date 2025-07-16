#pragma once
#include <map>
#include <schema/attributes/Partition.h>

class ResultsBuilder;
class SchemaMapResults
{
public:
  friend class ResultsBuilder;
  SchemaMapResults() = default;
  std::vector<std::string> summary() const
  {
    std::vector<std::string> ret;
    for (const auto &[key, res] : _m_resultsMap)
      ret.push_back(key);
    return ret;
  }

private:
  std::map<std::string, std::vector<unsigned char>> _m_resultsMap;
};

class ResultsBuilder
{
public:
  ResultsBuilder() {}
  void add(const Partition &part, char *buffer)
  {
    _m_results._m_resultsMap[std::string(part.key())] = std::vector<uint8_t>(part.length());
  }
  SchemaMapResults results() { return _m_results; }

private:
  SchemaMapResults _m_results;
};

std::ostream &operator<<(std::ostream &os, const SchemaMapResults &results)
{
  os << "Partitions: \n\t";
  for (const auto &partId : results.summary())
    os << partId << " ";
  os << '\n';
  return os;
}