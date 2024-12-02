#pragma once
#include <cstdint>
#include <type_traits>

namespace bm{

enum class SchemaFileStatus{
    VALID = 1,
    INVALID
};

class SchemaFileState{
public:
    [[nodiscard]] operator SchemaFileStatus() const noexcept{ return status_; }
    inline bool operator==(const SchemaFileState& rhs) const noexcept {
        return status_ == rhs.status_;
    }
    inline bool operator==(const SchemaFileStatus& status) const noexcept {
        return status_ == status;
    }
    [[nodiscard]] static SchemaFileState ValidState() noexcept { return SchemaFileState(SchemaFileStatus::VALID); }
    [[nodiscard]] static SchemaFileState InvalidState() noexcept { return SchemaFileState(SchemaFileStatus::INVALID); }
private:
    SchemaFileState(SchemaFileStatus status): status_{status}{}
    SchemaFileStatus status_;
};

}