#include "Schema.h"
#include <schema/attributes/Partition.h>
#include <algorithm>

void verifyOffsetOverlap(const Partition &first, const Partition &second)
{

  if (second.start_offset() > first.end_offset())
    return;
  std::stringstream ss;
  ss << "BinMorph ERR: Partition Overlap\n"
     << std::right
     << std::setw(20)
     << std::setfill(' ')
     << "Partition: " << second.key()
     << "\n";

  ss << std::right
     << std::setw(20)
     << std::setfill(' ')
     << "Partition: " << first.key()
     << "\n";
  std::cerr << ss.str() << std::endl;
  throw "";
}

Schema Schema::fromJSON(const nlohmann::json &json)
{
  Schema ret;

  for (const auto &[key, value] : json.items())
  {
    PartitionParameters params(key, value);
    auto part = Partition::Create(params);
    ret._m_parts.emplace_back(part);
  }

  auto schemaOffsetCmp = [](const Partition &p1, const Partition &p2)
  { return p1.start_offset() < p2.start_offset(); };
  auto schemaOffsetSort = [](const Partition &p1, const Partition &p2)
  {
    verifyOffsetOverlap(p1, p2);
    return true;
  };

  if (!std::is_sorted(ret._m_parts.begin(), ret._m_parts.end(), schemaOffsetCmp))
    std::sort(ret._m_parts.begin(), ret._m_parts.end(), schemaOffsetSort);

  return ret;
}