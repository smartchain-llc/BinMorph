#pragma once
#include <cstdlib>
#include <filesystem>
#include <concepts>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct schema_id_t {
    size_t id;
};

#ifdef __cplusplus
}
#endif

class Schema{
public:
template<typename IG>
Schema(std::filesystem::path& file, IG&& id)
    {
        _file = std::move(file);
        _id = id(*this);
    }
    operator struct schema_id_t(){
        return {_id};
    }
    const std::filesystem::path& filePath() const noexcept { return _file; }
    inline size_t id() const noexcept { return _id; }
private:
    size_t _id = 0;
    std::filesystem::path _file;
};

struct IdGenerator{
    size_t operator()(Schema&);
};