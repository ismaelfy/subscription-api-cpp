#pragma once
#include <string>

class PaymentMethod {
public:
    PaymentMethod(const std::string& id, const std::string& name, const std::string& description)
        : _id(id), _name(name), _description(description) {}

    const std::string& getId() const { return _id; }
    const std::string& getName() const { return _name; }
    const std::string& getDescription() const { return _description; }

    void setId(const std::string& id) { _id = id; }
    void setName(const std::string& name) { _name = name; }
    void setDescription(const std::string& description) { _description = description; }

private:
    std::string _id;
    std::string _name;
    std::string _description;
};
