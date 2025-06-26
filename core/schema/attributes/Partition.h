#pragma once
#include "FieldAttributes.h"
#include "Segment.h"
#include <iostream>
#include <string_view>
#include <vector>
#include <concepts>

class PartitionParameters {
public:
PartitionParameters() = default;
  PartitionParameters(const std::string& key, const nlohmann::json& json) {
    this->key = std::move(key);
    size_t o;
    json.at("offset").get_to(offset);

    fields = field_attributes(json["fields"]);
    if (json.contains("length"))
      json["length"].get_to(length);

    else {
      for (const auto &field : fields)
        length += field.length;
    }
  }

  std::string key;
  std::size_t offset;
  std::size_t length { 0 };
  std::vector<FieldAttribute> fields;
};

class Partition {
public:
  static Partition Create(PartitionParameters &params) {
    return Partition(std::move(params));
  }
  static Partition fromJSON(const nlohmann::json &json) {
    return Partition(json);
  }

public:
  inline std::string_view key() const noexcept { return _m_key; }
  inline const std::size_t &length() const noexcept { return _m_length; }
  inline const std::size_t &start_offset() const noexcept { return _m_offset; }
  inline const std::size_t end_offset() const noexcept {
    return _m_offset + _m_length - 1;
  }
  inline const std::size_t fieldCount() const noexcept {
    return _m_fields.size();
  }
  inline const std::vector<FieldAttribute> &getFields() const {
    return _m_fields;
  }

protected:
  Partition(const nlohmann::json &json);
  Partition(PartitionParameters&& params):
    _m_key{ std::move(params.key) },
    _m_offset{ std::move(params.offset) },
    _m_length{ std::move(params.length) },
    _m_fields{ std::move(params.fields) }
    {}

private:
  std::string _m_key;
  std::size_t _m_length{0};
  std::size_t _m_offset{0};
  std::vector<FieldAttribute> _m_fields;
};

// void from_json(const nlohmann::json &, PartitionParameters &);

std::ostream &operator<<(std::ostream &, const Partition &);


