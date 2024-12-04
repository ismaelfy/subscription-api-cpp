#pragma once
#include <string>

class Plan
{
public:
    Plan(const std::string &id, const std::string &name, double price, int durationDays)
        : _id(id), _name(name), _price(price), _durationDays(durationDays) {}

    const std::string &getId() const { return _id; }
    const std::string &getName() const { return _name; }
    double getPrice() const { return _price; }
    int getDurationDays() const { return _durationDays; }

    void setId(const std::string &id) { _id = id; }
    void setName(const std::string &name) { _name = name; }
    void setPrice(double price) { _price = price; }
    void setDurationDays(int durationDays) { _durationDays = durationDays; }

private:
    std::string _id;
    std::string _name;
    double _price;
    int _durationDays;
};
