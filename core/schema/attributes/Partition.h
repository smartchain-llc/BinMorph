#pragma once
#include "FieldAttributes.h"
#include "Segment.h"
#include <iostream>
#include <string_view>
#include <vector>
#include <concepts>

class PartitionParameters
{
public:
  PartitionParameters() = default;
  PartitionParameters(const std::string &key, const nlohmann::json &json)
  {
    this->key = std::move(key);
    size_t o;
    json.at("offset").get_to(offset);

    fields = field_attributes(json["fields"]);
    if (json.contains("length"))
      json["length"].get_to(length);

    else
    {
      for (const auto &field : fields)
        length += field.length;
    }
  }

  std::string key;
  std::size_t offset;
  std::size_t length{0};
  std::vector<FieldAttribute> fields;
};

class Partition
{
public:
  static Partition Create(PartitionParameters &params)
  {
    return Partition(std::move(params));
  }
  static Partition fromJSON(const nlohmann::json &json)
  {
    return Partition(json);
  }

public:
  bool operator==(const Partition &rhs) const noexcept
  {
    return this->_m_key == rhs._m_key && this->_m_offset == rhs._m_offset && this->_m_length == rhs._m_length;
  }
  class const_iterator
  {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = const Partition &;
    using type = Partition::const_iterator;
    using reference = const Partition &;
    using pointer = const Partition *;
    using iterator_category = std::forward_iterator_tag;

    const_iterator() = default;
    const_iterator(const const_iterator &rhs)
    {
      _m_partContainer = rhs._m_partContainer;
      _m_pos = rhs._m_pos;
    }

    const_iterator(const std::vector<Partition> *parts, std::size_t at)
    {
      _m_partContainer = parts;
      _m_pos = at;
    }

    value_type operator*() const { return _m_partContainer->data()[_m_pos]; }
    const_iterator &operator++()
    {
      _m_pos++;
      return *this;
    }
    void operator++(int) { ++*this; }

    bool operator==(const const_iterator &rhs) const
    {
      return **this == *rhs;
    }
    bool operator!=(const const_iterator &rhs) const
    {
      return !(**this == *rhs);
    }

  private:
    const std::vector<Partition> *_m_partContainer;
    std::size_t _m_pos{0};
  };

public:
  inline std::string_view key() const noexcept { return _m_key; }
  inline const std::size_t &length() const noexcept { return _m_length; }
  inline const std::size_t &start_offset() const noexcept { return _m_offset; }
  inline const std::size_t end_offset() const noexcept
  {
    return _m_offset + _m_length - 1;
  }
  inline const std::size_t fieldCount() const noexcept
  {
    return _m_fields.size();
  }
  inline const std::vector<FieldAttribute> &getFields() const
  {
    return _m_fields;
  }

protected:
  Partition(const nlohmann::json &json);
  Partition(PartitionParameters &&params) : _m_key{std::move(params.key)},
                                            _m_offset{std::move(params.offset)},
                                            _m_length{std::move(params.length)},
                                            _m_fields{std::move(params.fields)}
  {
  }

private:
  std::string _m_key;
  std::size_t _m_length{0};
  std::size_t _m_offset{0};
  std::vector<FieldAttribute> _m_fields;
};

// void from_json(const nlohmann::json &, PartitionParameters &);

std::ostream &operator<<(std::ostream &, const Partition &);
