#pragma once
#include <schema/Schema.h>

namespace bm
{

    class ToJSONMapper
    {
    public:
        using ResultsType = nlohmann::json;
        void map(const Schema &schema, uint8_t *dataProvider);
        const nlohmann::json& results() const noexcept { return _m_results; }
    private:
        nlohmann::json _m_results;
    };
}