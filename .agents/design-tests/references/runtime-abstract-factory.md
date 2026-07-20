# Runtime Abstract Factory Reference

## Intent

Provide an interface for creating a cohesive family of polymorphic products without coupling the consumer to concrete product classes or constructor details.

## Core interfaces

```cpp
struct Request {
    int id;
    std::string body;
};

class ILogger {
public:
    virtual ~ILogger() {}
    virtual void info(const std::string& message) = 0;
    virtual void error(const std::string& message) = 0;
};

class ISerializer {
public:
    virtual ~ISerializer() {}
    virtual std::string serialize(const Request& request) = 0;
};

class ITransport {
public:
    virtual ~ITransport() {}
    virtual bool connect() = 0;
    virtual bool send(const std::string& payload) = 0;
};

class IRuntimeFactory {
public:
    virtual ~IRuntimeFactory() {}
    virtual std::unique_ptr<ILogger> create_logger() const = 0;
    virtual std::unique_ptr<ISerializer> create_serializer() const = 0;
    virtual std::unique_ptr<ITransport> create_transport() const = 0;
};
```

## Production factory

```cpp
class ProductionRuntimeFactory : public IRuntimeFactory {
public:
    ProductionRuntimeFactory(std::ostream& output,
                             std::string host,
                             unsigned short port)
        : output_(output), host_(std::move(host)), port_(port) {}

    std::unique_ptr<ILogger> create_logger() const override {
        return std::unique_ptr<ILogger>(new StreamLogger(output_));
    }

    std::unique_ptr<ISerializer> create_serializer() const override {
        return std::unique_ptr<ISerializer>(new JsonSerializer());
    }

    std::unique_ptr<ITransport> create_transport() const override {
        return std::unique_ptr<ITransport>(new TcpTransport(host_, port_));
    }

private:
    std::ostream& output_;
    std::string host_;
    unsigned short port_;
};
```

## Consumer

```cpp
class RequestProcessor {
public:
    explicit RequestProcessor(const IRuntimeFactory& factory)
        : factory_(factory) {}

    bool process(const Request& request) {
        std::unique_ptr<ILogger> logger = factory_.create_logger();
        std::unique_ptr<ISerializer> serializer = factory_.create_serializer();
        std::unique_ptr<ITransport> transport = factory_.create_transport();

        if (!logger || !serializer || !transport) {
            throw std::runtime_error("factory returned null product");
        }

        const std::string payload = serializer->serialize(request);
        if (payload.empty()) {
            logger->error("Serialization failed");
            return false;
        }

        if (!transport->connect()) {
            logger->error("Connection failed");
            return false;
        }

        if (!transport->send(payload)) {
            logger->error("Send failed");
            return false;
        }

        logger->info("Request sent");
        return true;
    }

private:
    const IRuntimeFactory& factory_;
};
```

## Contract choices

Choose and document one failure policy:

1. **Non-null contract:** factory creation either returns a valid product or throws. This simplifies consumers.
2. **Nullable contract:** creation may return null. Every consumer must handle and tests must cover each short-circuit operand.
3. **Expected/result contract:** return a project result type containing either a product or typed construction error.

Avoid ambiguous mixed policies.

## When Abstract Factory is justified

Use it when at least one is true:

- Multiple related products must be selected together.
- Runtime configuration selects a product family.
- Implementations cross a library/ABI/plugin boundary.
- Product constructors are complex or unstable.
- Product lifetimes are per operation and must remain local.
- Tests need controlled product creation, fault injection, or observation.

For only one simple product, a Factory Method, injected callable, or direct dependency may be cleaner.

## Callable alternative for one product

```cpp
using ReaderCreator = std::function<std::unique_ptr<IReader>(const Path&)>;

class Loader {
public:
    explicit Loader(ReaderCreator creator)
        : creator_(std::move(creator)) {}

    Data load(const Path& path) {
        std::unique_ptr<IReader> reader = creator_(path);
        return reader->read();
    }

private:
    ReaderCreator creator_;
};
```

Do not introduce an abstract factory hierarchy merely to wrap one trivial constructor.
