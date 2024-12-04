#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <typeinfo>

class Specification {
public:
    explicit Specification(const std::string& key) : key_(key) {}
    // virtual ~Specification() = default;
    std::string key() const { return key_; }

private:
    std::string key_;
};

struct HeaderData {
    std::size_t len;
};
class HeaderSpec : public Specification {
public:
    HeaderSpec() : Specification("header") {}
    void fromJSON(const nlohmann::json& json){
        std::cout << json << std::endl;
        data.len = json.find("length").value();
        std::cout << "data.len = " << json.find("length").value() << std::endl; 
    }
    HeaderData data;
};

class FooterSpec : public Specification {
public:
    FooterSpec() : Specification("footer") {}
};

class SpecificationWrapper {
public:
    template <typename T>
    SpecificationWrapper(T spec) : impl_(std::make_shared<Model<T>>(std::move(spec))) {}

    template <typename T>
    T as() const {
        if (typeid(T) != impl_->type()) {
            throw std::bad_cast();
        }
        return static_cast<Model<T>*>(impl_.get())->data_;
    }

    const std::type_info& type() const { return impl_->type(); }

private:
    struct Concept {
        virtual ~Concept() = default;
        virtual const std::type_info& type() const = 0;
    };

    template <typename T>
    struct Model : Concept {
        Model(T data) : data_(std::move(data)) {}
        const std::type_info& type() const override { return typeid(T); }
        T data_;
    };

    std::shared_ptr<Concept> impl_;
};