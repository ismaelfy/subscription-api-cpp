#pragma once
#include <string>

class Payment
{
public:
    Payment(const std::string &id, const std::string &userId, const std::string &paymentMethodId, double amount, const std::string &date, const std::string &status)
        : _id(id), _userId(userId), _paymentMethodId(paymentMethodId), _amount(amount), _date(date), _status(status) {}

    const std::string &getId() const { return _id; }
    const std::string &getUserId() const { return _userId; }
    const std::string &getPaymentMethodId() const { return _paymentMethodId; }
    double getAmount() const { return _amount; }
    const std::string &getDate() const { return _date; }
    const std::string &getStatus() const { return _status; }

    void setId(const std::string &id) { _id = id; }
    void setUserId(const std::string &userId) { _userId = userId; }
    void setPaymentMethodId(const std::string &paymentMethodId) { _paymentMethodId = paymentMethodId; }
    void setAmount(double amount) { _amount = amount; }
    void setDate(const std::string &date) { _date = date; }
    void setStatus(const std::string &status) { _status = status; }

private:
    std::string _id;
    std::string _userId;
    std::string _paymentMethodId;
    double _amount;
    std::string _date;
    std::string _status;
};
