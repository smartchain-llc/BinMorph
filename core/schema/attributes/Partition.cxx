#include "Partition.h"
#include "json.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
// Partition::Partition(PartitionParameters&& params){
//   _m_offset = std::move(params.offset);
//   _m_length = std::move(params.length);
//   _m_fields = std::move(params.fields);
//   _m_key = std::move(params.key);
// }
Partition::Partition(const nlohmann::json &json) {

  json.at("offset").get_to(_m_offset);

  _m_fields = field_attributes(json["fields"]);

  if (json.contains("length"))
    json["length"].get_to(_m_length);
  else {
    for (const auto &field : _m_fields)
      _m_length += field.length;
  }
}

// void from_json(const nlohmann::json &json, PartitionParameters &dest) {}

std::ostream &operator<<(std::ostream &ostream, const Partition &part) {
  std::stringstream ss;
  std::string key{part.key()};

  std::transform(key.begin(), key.end(), key.begin(),
                 [](unsigned char c) { return std::toupper(c); });

  ss << "Partition: " << key << " ( " << part.fieldCount() << " )" << '\n';

  ss << "=========================" << std::endl;

  ss << std::right << std::setw(15) << std::setfill(' ') << "offset | ";
  ss << std::left << part.start_offset() << std::right << std::setw(15)
     << std::setfill('.') << part.end_offset() << '\n';

  ss << std::right << std::setw(15) << std::setfill(' ') << "length | ";
  ss << std::left << part.length() << "\n";

  ss << std::right << std::setw(15) << std::setfill(' ') << "fields | ";

  ss << "\n";

  for (const auto &field : part.getFields()) {
    ss << std::left << std::setw(20) << std::setfill(' ');

    ss << std::right << std::setw(20) << "Field: " << field.name << "\n";

    ss << std::right << std::setw(30) << std::setfill(' ') << "length | ";
    ss << std::left << field.length << "\n";

    ss << std::right << std::setw(30) << std::setfill(' ') << "endianness | ";

    std::string endianUpper{field.endian};
    std::transform(endianUpper.begin(), endianUpper.end(), endianUpper.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    ss << std::left << endianUpper << "\n";
  }
  ostream << ss.str();
  return ostream;
}