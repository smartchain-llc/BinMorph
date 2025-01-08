#pragma once
#include <bm/schema.h>
namespace bm{

namespace traits{

/**
Concept to satisfy for classes that implement a "mapper" functionality. That being, mapping a data source against a
JSON schema.

## Examples
#### Default Constructible
```
struct MemMapper{
    uint8_t data[1024];
    void map(bm::Schema& s, uint8_t* buf, std::size_t len){
        // Implementation
    }
};

int main(){
    const auto bm::create_schema(...);
    uint8_t inputBuffer[2048];

    bm::Map<MemMapper>(schema, inputBuffer, 2048);  // OK: Resolves
    bm::Map<MemMapper>(schema, inputBuffer);        // ERR: No implementation found w/o `std::size_t` arg
}
```

#### Non Default Constructible
```

struct NonDefaultMapper {
    explicit NonDefaultMapper(bool v){}
    void map(bm::Schema s, uint8_t* buf, std::size_t len){
        // Implementation
    }
};
int main(){
    const auto bm::create_schema(...);
    uint8_t inputBuffer[2048];

    NonDefaultMapper explicitMapper{ false };
    
    bm::Map<NonDefaultMapper>(explicitMapper, schema, inputBuffer, 2048);  // OK: Resolves
    bm::Map<NonDefaultMapper>(schema, inputBuffer);        // ERR: No implementation found for this function signature
    bm::Map(explicitMapper, schema, inputBuffer, 2048);         // OK: Resolves
}

```
*/
template<typename M, typename S, typename ...Args> 
concept is_mappable = bm::clean::is_schema_type<S> && requires(M m, S s, Args... args){
    m.map(s, args...);
} ;

}

template<typename M, bm::clean::is_schema_type S, typename...Args>
std::enable_if_t<
    traits::is_mappable<M, S, Args...> && std::is_default_constructible_v<M>,
    void
> Map(S s, Args... args){
    M{}.map(s, args...);
}


template<typename M, bm::clean::is_schema_type S, typename...Args>
std::enable_if_t<
    traits::is_mappable<M, S, Args...> && !std::is_default_constructible_v<M>,
    void
> Map(M& m, S s, Args... args){
    m.map(s, args...);
}
}