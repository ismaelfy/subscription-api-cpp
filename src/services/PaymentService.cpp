#include "PaymentService.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <mongocxx/exception/exception.hpp>
#include <iostream>

PaymentService::PaymentService(MongoConnection &connection) : _connection(connection) {}

std::string PaymentService::createPayment(const Payment &payment)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payments"];

    bsoncxx::builder::basic::document document{};
    document.append(bsoncxx::builder::basic::kvp("userId", payment.getUserId()));
    document.append(bsoncxx::builder::basic::kvp("paymentMethodId", payment.getPaymentMethodId()));
    document.append(bsoncxx::builder::basic::kvp("amount", payment.getAmount()));
    document.append(bsoncxx::builder::basic::kvp("date", payment.getDate()));
    document.append(bsoncxx::builder::basic::kvp("status", payment.getStatus()));

    auto result = collection.insert_one(document.view());
    return result->inserted_id().get_oid().value.to_string();
}

std::optional<Payment> PaymentService::getPaymentById(const std::string &id)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payments"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    auto doc = collection.find_one(query.view());
    if (doc)
    {
        auto view = doc->view();
        return Payment(
            view["_id"].get_oid().value.to_string(),
            std::string(view["userId"].get_string().value),
            std::string(view["paymentMethodId"].get_string().value),
            view["amount"].get_double().value,
            std::string(view["date"].get_string().value),
            std::string(view["status"].get_string().value));
    }
    return std::nullopt;
}

std::vector<Payment> PaymentService::getAllPayments()
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payments"];

    std::vector<Payment> payments;
    auto cursor = collection.find({});
    for (const auto &doc : cursor)
    {
        auto view = doc;
        payments.emplace_back(
            view["_id"].get_oid().value.to_string(),
            std::string(view["userId"].get_string().value),
            std::string(view["paymentMethodId"].get_string().value),
            view["amount"].get_double().value,
            std::string(view["date"].get_string().value),
            std::string(view["status"].get_string().value));
    }
    return payments;
}

bool PaymentService::updatePayment(const std::string &id, const Payment &payment)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payments"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    bsoncxx::builder::basic::document set_doc{};
    set_doc.append(bsoncxx::builder::basic::kvp("userId", payment.getUserId()));
    set_doc.append(bsoncxx::builder::basic::kvp("paymentMethodId", payment.getPaymentMethodId()));
    set_doc.append(bsoncxx::builder::basic::kvp("amount", payment.getAmount()));
    set_doc.append(bsoncxx::builder::basic::kvp("date", payment.getDate()));
    set_doc.append(bsoncxx::builder::basic::kvp("status", payment.getStatus()));

    bsoncxx::builder::basic::document update_doc{};
    update_doc.append(bsoncxx::builder::basic::kvp("$set", set_doc.view()));

    auto result = collection.update_one(query.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

bool PaymentService::deletePayment(const std::string &id)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payments"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    auto result = collection.delete_one(query.view());
    return result && result->deleted_count() > 0;
}
