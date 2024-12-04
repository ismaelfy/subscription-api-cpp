#pragma once
#include "../models/Payment.h"
#include "../database/MongoConnection.h"
#include <vector>
#include <optional>

class PaymentService
{
public:
    PaymentService(MongoConnection &connection);

    std::string createPayment(const Payment &payment);
    std::optional<Payment> getPaymentById(const std::string &id);
    std::vector<Payment> getAllPayments();
    bool updatePayment(const std::string &id, const Payment &payment);
    bool deletePayment(const std::string &id);

private:
    MongoConnection &_connection;
};
