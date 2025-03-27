#include "Partition.h"

Partition::Partition(const nlohmann::json &json)
{
    key = json.items().begin().key();
    auto attr = json.front();

    attr["offset"].get_to(offset);
    attr["length"].get_to(length);
    fields = field_attributes(attr["fields"]);
}

template <typename V>
Partition &&CreateFrom(V &v, const nlohmann::json &json)
{
    if (!v(json))
        throw "PERR";
    return std::move(Partition{json});
}