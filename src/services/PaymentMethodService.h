#pragma once
#include "../models/PaymentMethod.h"
#include "../database/MongoConnection.h"
#include <vector>
#include <optional>

class PaymentMethodService {
public:
    PaymentMethodService(MongoConnection& connection);

    std::string createPaymentMethod(const PaymentMethod& paymentMethod);
    std::optional<PaymentMethod> getPaymentMethodById(const std::string& id);
    std::vector<PaymentMethod> getAllPaymentMethods();
    bool updatePaymentMethod(const std::string& id, const PaymentMethod& paymentMethod);
    bool deletePaymentMethod(const std::string& id);

private:
    MongoConnection& _connection;
};
