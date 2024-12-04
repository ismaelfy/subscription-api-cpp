#include "PaymentController.h"

PaymentController::PaymentController(PaymentService &service) : _paymentService(service) {}

std::optional<std::string> PaymentController::handleCreatePayment(const Payment &payment)
{
    return _paymentService.createPayment(payment);
}

std::optional<Payment> PaymentController::handleGetPaymentById(const std::string &id)
{
    return _paymentService.getPaymentById(id);
}

std::vector<Payment> PaymentController::handleGetAllPayments()
{
    return _paymentService.getAllPayments();
}

bool PaymentController::handleUpdatePayment(const std::string &id, const Payment &payment)
{
    return _paymentService.updatePayment(id, payment);
}

bool PaymentController::handleDeletePayment(const std::string &id)
{
    return _paymentService.deletePayment(id);
}
