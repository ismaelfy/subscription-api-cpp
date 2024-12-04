#pragma once
#include "../services/PaymentMethodService.h"
#include "../models/PaymentMethod.h"
#include <optional>

class PaymentMethodController
{
public:
    PaymentMethodController(PaymentMethodService &service);

    std::optional<std::string> handleCreatePaymentMethod(const PaymentMethod &paymentMethod);
    std::optional<PaymentMethod> handleGetPaymentMethodById(const std::string &id);
    std::vector<PaymentMethod> handleGetAllPaymentMethods();
    bool handleUpdatePaymentMethod(const std::string &id, const PaymentMethod &paymentMethod);
    bool handleDeletePaymentMethod(const std::string &id);

private:
    PaymentMethodService &_paymentMethodService;
};
