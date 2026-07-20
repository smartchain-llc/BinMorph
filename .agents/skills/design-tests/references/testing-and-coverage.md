# Testing and Coverage Reference

## Runtime test-state pattern

Products returned as `unique_ptr` are destroyed before assertions after `process()` returns. Store observations in shared state owned by the test.

```cpp
struct RuntimeTestState {
    bool connect_result = true;
    bool send_result = true;
    std::string serialized_value;
    int logger_created = 0;
    int serializer_created = 0;
    int transport_created = 0;
    int serialize_calls = 0;
    int connect_calls = 0;
    int send_calls = 0;
    std::string last_payload;
    std::vector<std::string> infos;
    std::vector<std::string> errors;
};
```

For strict C++11 portability, replace in-class initializers if the project's compiler policy requires it.

## Test products

```cpp
class RecordingLogger : public ILogger {
public:
    explicit RecordingLogger(RuntimeTestState& state) : state_(state) {}
    void info(const std::string& value) override { state_.infos.push_back(value); }
    void error(const std::string& value) override { state_.errors.push_back(value); }
private:
    RuntimeTestState& state_;
};

class StubSerializer : public ISerializer {
public:
    explicit StubSerializer(RuntimeTestState& state) : state_(state) {}
    std::string serialize(const Request&) override {
        ++state_.serialize_calls;
        return state_.serialized_value;
    }
private:
    RuntimeTestState& state_;
};

class StubTransport : public ITransport {
public:
    explicit StubTransport(RuntimeTestState& state) : state_(state) {}
    bool connect() override {
        ++state_.connect_calls;
        return state_.connect_result;
    }
    bool send(const std::string& payload) override {
        ++state_.send_calls;
        state_.last_payload = payload;
        return state_.send_result;
    }
private:
    RuntimeTestState& state_;
};
```

## Required behavior matrix

| Scenario | Expected serialization | connect | send | result/log |
|---|---:|---:|---:|---|
| Success | 1 | 1 | 1 | true + info |
| Empty serialization | 1 | 0 | 0 | false + serialization error |
| Connection failure | 1 | 1 | 0 | false + connection error |
| Send failure | 1 | 1 | 1 | false + send error |
| Factory construction failure | depends on contract | 0 | 0 | exception/error result |

Also test creation count and constructor/configuration arguments when product identity or configuration matters.

## GoogleTest examples

```cpp
TEST(RequestProcessorTest, StopsAfterConnectionFailure) {
    RuntimeTestState state;
    state.serialized_value = "payload";
    state.connect_result = false;

    TestRuntimeFactory factory(state);
    RequestProcessor processor(factory);

    EXPECT_FALSE(processor.process(Request{42, "body"}));
    EXPECT_EQ(1, state.serialize_calls);
    EXPECT_EQ(1, state.connect_calls);
    EXPECT_EQ(0, state.send_calls);
    ASSERT_EQ(1u, state.errors.size());
    EXPECT_EQ("Connection failed", state.errors[0]);
}
```

## Short-circuit branch coverage

For:

```cpp
if (!logger || !serializer || !transport) { ... }
```

one null-logger test does not evaluate later operands. Add separate tests where:

1. logger is non-null and serializer is null;
2. logger and serializer are non-null and transport is null;
3. all are non-null.

Alternatively, make the factory contract non-null and remove the compound condition.

## Compile-time contract tests

Use both:

- Positive/negative `static_assert` trait tests in a normal target.
- An expected compile-failure translation unit for invalid processor instantiation.

A CTest compile-failure test:

```cmake
add_test(
    NAME invalid_factory_must_not_compile
    COMMAND ${CMAKE_CXX_COMPILER}
            -std=c++11
            -I${PROJECT_SOURCE_DIR}/include
            -c ${CMAKE_CURRENT_SOURCE_DIR}/invalid_factory.cpp
            -o ${CMAKE_CURRENT_BINARY_DIR}/invalid_factory.o
)
set_tests_properties(invalid_factory_must_not_compile PROPERTIES WILL_FAIL TRUE)
```

For stronger validation, run the compiler through a CMake script and search stderr for the custom assertion message.

## Coverage instrumentation

```cmake
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(core PRIVATE -O0 -g --coverage)
    target_compile_options(unit_tests PRIVATE -O0 -g --coverage)
    target_link_options(unit_tests PRIVATE --coverage)
endif()
```

Older CMake:

```cmake
set_target_properties(unit_tests PROPERTIES LINK_FLAGS "--coverage")
```

Typical commands:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
lcov --capture --directory build --output-file coverage.info --rc branch_coverage=1
lcov --remove coverage.info '/usr/*' '*/_deps/*' '*/tests/*' \
  --output-file coverage.filtered.info --rc branch_coverage=1
genhtml coverage.filtered.info --branch-coverage --output-directory coverage-html
```

## Coverage interpretation

- Line coverage: statements executed.
- Function coverage: generated functions entered.
- Branch coverage: decisions and compiler-generated branches exercised.
- Template code may produce multiple function records for one source definition.
- Inline destructors, exception cleanup, and short-circuit logic may create branches that require inspection rather than blind percentage chasing.
- Compile-only `static_assert` tests validate constraints but add no runtime execution counts.
- Coverage should guide missing test scenarios, not force tests of meaningless compiler artifacts.
