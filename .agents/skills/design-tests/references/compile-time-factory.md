# Compile-Time Factory and Policy Reference

## Intent

Select and validate a product family during compilation. This removes virtual dispatch and usually avoids heap allocation, while retaining a test substitution seam through an alternate factory type.

This is analogous to Abstract Factory but is more accurately described as a static factory or factory policy.

## C++11 detection utilities

```cpp
template <typename...>
struct make_void { typedef void type; };

template <typename... T>
using void_t = typename make_void<T...>::type;
```

## Factory creation-function detection

```cpp
template <typename Factory, typename = void>
struct is_request_factory : std::false_type {};

template <typename Factory>
struct is_request_factory<Factory, void_t<
    decltype(std::declval<Factory&>().create_logger()),
    decltype(std::declval<Factory&>().create_serializer()),
    decltype(std::declval<Factory&>().create_transport())
> > : std::true_type {};
```

## Product-operation detection

```cpp
template <typename Logger, typename = void>
struct is_logger : std::false_type {};

template <typename Logger>
struct is_logger<Logger, void_t<
    decltype(std::declval<Logger&>().info(
        std::declval<const std::string&>())),
    decltype(std::declval<Logger&>().error(
        std::declval<const std::string&>()))
> > : std::true_type {};

template <typename Serializer, typename = void>
struct is_serializer : std::false_type {};

template <typename Serializer>
struct is_serializer<Serializer, void_t<
    decltype(std::declval<Serializer&>().serialize(
        std::declval<const Request&>()))
> > : std::integral_constant<bool,
    std::is_convertible<
        decltype(std::declval<Serializer&>().serialize(
            std::declval<const Request&>())),
        std::string
    >::value
> {};

template <typename Transport, typename = void>
struct is_transport : std::false_type {};

template <typename Transport>
struct is_transport<Transport, void_t<
    decltype(std::declval<Transport&>().connect()),
    decltype(std::declval<Transport&>().send(
        std::declval<const std::string&>()))
> > : std::integral_constant<bool,
    std::is_convertible<
        decltype(std::declval<Transport&>().connect()), bool
    >::value &&
    std::is_convertible<
        decltype(std::declval<Transport&>().send(
            std::declval<const std::string&>())), bool
    >::value
> {};
```

## Product type extraction

```cpp
template <typename Factory>
struct factory_products {
    typedef decltype(std::declval<Factory&>().create_logger()) logger_type;
    typedef decltype(std::declval<Factory&>().create_serializer()) serializer_type;
    typedef decltype(std::declval<Factory&>().create_transport()) transport_type;
};
```

## Constrained consumer

```cpp
template <typename Factory>
class RequestProcessorT {
private:
    typedef factory_products<Factory> products;

    static_assert(is_request_factory<Factory>::value,
                  "Factory must provide create_logger(), "
                  "create_serializer(), and create_transport()");
    static_assert(is_logger<typename products::logger_type>::value,
                  "create_logger() must return a logger-like type");
    static_assert(is_serializer<typename products::serializer_type>::value,
                  "create_serializer() must return a serializer-like type");
    static_assert(is_transport<typename products::transport_type>::value,
                  "create_transport() must return a transport-like type");

public:
    explicit RequestProcessorT(Factory factory)
        : factory_(std::move(factory)) {}

    bool process(const Request& request) {
        typename products::logger_type logger = factory_.create_logger();
        typename products::serializer_type serializer = factory_.create_serializer();
        typename products::transport_type transport = factory_.create_transport();

        const std::string payload = serializer.serialize(request);
        if (payload.empty()) {
            logger.error("Serialization failed");
            return false;
        }
        if (!transport.connect()) {
            logger.error("Connection failed");
            return false;
        }
        if (!transport.send(payload)) {
            logger.error("Send failed");
            return false;
        }
        logger.info("Request sent");
        return true;
    }

private:
    Factory factory_;
};
```

## Positive and negative trait tests

```cpp
static_assert(is_request_factory<ProductionStaticFactory>::value,
              "production factory must satisfy creation contract");
static_assert(is_logger<ValueLogger>::value,
              "logger contract regression");

struct MissingErrorLogger {
    void info(const std::string&);
};
static_assert(!is_logger<MissingErrorLogger>::value,
              "invalid logger must be rejected");
```

## Expected compile-failure test

Place invalid instantiation in a separate translation unit and compile it from CTest or `try_compile`. Check that compilation fails and, where practical, that stderr includes the custom `static_assert` diagnostic.

## Constraints and tradeoffs

- Every factory type creates a distinct template instantiation.
- Production and test instantiations can appear separately in gcov/genhtml.
- Definitions generally live in headers, increasing recompilation and implementation exposure.
- Static factories cannot be selected from runtime configuration without a runtime adapter/type-erasure layer.
- Structural contracts can accidentally accept semantically wrong operations; names and return types are necessary but not sufficient. Runtime tests remain required.
