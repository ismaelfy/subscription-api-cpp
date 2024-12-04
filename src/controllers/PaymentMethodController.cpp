#include "PaymentMethodController.h"

PaymentMethodController::PaymentMethodController(PaymentMethodService &service)
    : _paymentMethodService(service) {}

std::optional<std::string> PaymentMethodController::handleCreatePaymentMethod(const PaymentMethod &paymentMethod)
{
    return _paymentMethodService.createPaymentMethod(paymentMethod);
}

std::optional<PaymentMethod> PaymentMethodController::handleGetPaymentMethodById(const std::string &id)
{
    return _paymentMethodService.getPaymentMethodById(id);
}

std::vector<PaymentMethod> PaymentMethodController::handleGetAllPaymentMethods()
{
    return _paymentMethodService.getAllPaymentMethods();
}

bool PaymentMethodController::handleUpdatePaymentMethod(const std::string &id, const PaymentMethod &paymentMethod)
{
    return _paymentMethodService.updatePaymentMethod(id, paymentMethod);
}

bool PaymentMethodController::handleDeletePaymentMethod(const std::string &id)
{
    return _paymentMethodService.deletePaymentMethod(id);
}
