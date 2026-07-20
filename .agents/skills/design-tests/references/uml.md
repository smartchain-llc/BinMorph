# UML Reference

These Mermaid diagrams are source-controlled UML documentation. Update names and operations to match the repository rather than copying them blindly.

## Runtime Abstract Factory — class diagram

```mermaid
classDiagram
    direction LR

    class RequestProcessor {
        -IRuntimeFactory& factory_
        +RequestProcessor(IRuntimeFactory&)
        +process(Request) bool
    }

    class IRuntimeFactory {
        <<interface>>
        +createLogger() unique_ptr~ILogger~
        +createSerializer() unique_ptr~ISerializer~
        +createTransport() unique_ptr~ITransport~
    }

    class ProductionRuntimeFactory {
        -Configuration config_
        +createLogger() unique_ptr~ILogger~
        +createSerializer() unique_ptr~ISerializer~
        +createTransport() unique_ptr~ITransport~
    }

    class TestRuntimeFactory {
        -RuntimeTestState& state_
        +createLogger() unique_ptr~ILogger~
        +createSerializer() unique_ptr~ISerializer~
        +createTransport() unique_ptr~ITransport~
    }

    class ILogger {
        <<interface>>
        +info(string)
        +error(string)
    }

    class ISerializer {
        <<interface>>
        +serialize(Request) string
    }

    class ITransport {
        <<interface>>
        +connect() bool
        +send(string) bool
    }

    class FileLogger
    class JsonSerializer
    class TcpTransport
    class RecordingLogger
    class StubSerializer
    class StubTransport

    RequestProcessor --> IRuntimeFactory : requests products
    IRuntimeFactory <|.. ProductionRuntimeFactory
    IRuntimeFactory <|.. TestRuntimeFactory

    ILogger <|.. FileLogger
    ILogger <|.. RecordingLogger
    ISerializer <|.. JsonSerializer
    ISerializer <|.. StubSerializer
    ITransport <|.. TcpTransport
    ITransport <|.. StubTransport

    ProductionRuntimeFactory ..> FileLogger : creates
    ProductionRuntimeFactory ..> JsonSerializer : creates
    ProductionRuntimeFactory ..> TcpTransport : creates
    TestRuntimeFactory ..> RecordingLogger : creates
    TestRuntimeFactory ..> StubSerializer : creates
    TestRuntimeFactory ..> StubTransport : creates
```

## Runtime processing — sequence diagram

```mermaid
sequenceDiagram
    autonumber
    actor Caller
    participant Processor as RequestProcessor
    participant Factory as IRuntimeFactory
    participant Logger as ILogger
    participant Serializer as ISerializer
    participant Transport as ITransport

    Caller->>Processor: process(request)
    Processor->>Factory: createLogger()
    Factory-->>Processor: unique_ptr<ILogger>
    Processor->>Factory: createSerializer()
    Factory-->>Processor: unique_ptr<ISerializer>
    Processor->>Factory: createTransport()
    Factory-->>Processor: unique_ptr<ITransport>
    Processor->>Serializer: serialize(request)
    Serializer-->>Processor: payload

    alt payload empty
        Processor->>Logger: error("Serialization failed")
        Processor-->>Caller: false
    else payload valid
        Processor->>Transport: connect()
        alt connection fails
            Processor->>Logger: error("Connection failed")
            Processor-->>Caller: false
        else connected
            Processor->>Transport: send(payload)
            alt send fails
                Processor->>Logger: error("Send failed")
                Processor-->>Caller: false
            else send succeeds
                Processor->>Logger: info("Request sent")
                Processor-->>Caller: true
            end
        end
    end
```

## Compile-time factory — class diagram

```mermaid
classDiagram
    direction LR

    class RequestProcessorT~Factory~ {
        -Factory factory_
        +process(Request) bool
    }

    class ProductionStaticFactory {
        +createLogger() ValueLogger
        +createSerializer() ValueSerializer
        +createTransport() ValueTransport
    }

    class StaticTestFactory {
        +createLogger() TestLogger
        +createSerializer() TestSerializer
        +createTransport() TestTransport
    }

    class FactoryContract {
        <<template constraint>>
        createLogger()
        createSerializer()
        createTransport()
    }

    class LoggerContract {
        <<structural contract>>
        info(string)
        error(string)
    }

    class SerializerContract {
        <<structural contract>>
        serialize(Request) convertible_to string
    }

    class TransportContract {
        <<structural contract>>
        connect() convertible_to bool
        send(string) convertible_to bool
    }

    RequestProcessorT~Factory~ ..> FactoryContract : static_assert
    ProductionStaticFactory ..|> FactoryContract
    StaticTestFactory ..|> FactoryContract
    FactoryContract ..> LoggerContract : returned product
    FactoryContract ..> SerializerContract : returned product
    FactoryContract ..> TransportContract : returned product
```

## Incremental legacy migration — component view

```mermaid
flowchart LR
    LegacyCaller[Existing callers] --> Wrapper[Legacy-compatible production wrapper]
    NewCaller[New composition code] --> Core[Testable core]
    Wrapper --> ProductionFactory[Production factory]
    Wrapper --> Core
    Tests[Unit tests] --> TestFactory[Test factory/policy]
    TestFactory --> Core
    ProductionFactory --> Adapters[Concrete I/O adapters]
    Core --> Interfaces[Behavioral interfaces or static contracts]
```

## Ownership model

```mermaid
flowchart TD
    Root[Composition root owns long-lived factory/configuration]
    Root --> Consumer[Consumer stores factory reference/value]
    Consumer -->|per operation| Factory[Factory creates transient product]
    Factory --> Product[unique_ptr product or value product]
    Product -->|scope exit| Destroy[Deterministic destruction]
```

Use aggregation/composition carefully:

- A reference member means the consumer does not own the factory.
- A factory returned as a value is owned by the consumer.
- `unique_ptr<Product>` transfers sole ownership to the caller.
- A shared test-state object may outlive transient test products so assertions remain possible after product destruction.
