#pragma once
#include "../services/PaymentService.h"
#include "../models/Payment.h"
#include <optional>

class PaymentController
{
public:
    PaymentController(PaymentService &service);

    std::optional<std::string> handleCreatePayment(const Payment &payment);
    std::optional<Payment> handleGetPaymentById(const std::string &id);
    std::vector<Payment> handleGetAllPayments();
    bool handleUpdatePayment(const std::string &id, const Payment &payment);
    bool handleDeletePayment(const std::string &id);

private:
    PaymentService &_paymentService;
};
