#include "PaymentMethodService.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <mongocxx/exception/exception.hpp>
#include <iostream>

PaymentMethodService::PaymentMethodService(MongoConnection &connection) : _connection(connection) {}

std::string PaymentMethodService::createPaymentMethod(const PaymentMethod &paymentMethod)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payment_methods"];

    bsoncxx::builder::basic::document document{};
    document.append(bsoncxx::builder::basic::kvp("name", paymentMethod.getName()));
    document.append(bsoncxx::builder::basic::kvp("description", paymentMethod.getDescription()));

    auto result = collection.insert_one(document.view());
    return result->inserted_id().get_oid().value.to_string();
}

std::optional<PaymentMethod> PaymentMethodService::getPaymentMethodById(const std::string &id)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payment_methods"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    auto doc = collection.find_one(query.view());
    if (doc)
    {
        auto view = doc->view();
        return PaymentMethod(
            view["_id"].get_oid().value.to_string(),
            std::string(view["name"].get_string().value),
            std::string(view["description"].get_string().value));
    }
    return std::nullopt;
}

std::vector<PaymentMethod> PaymentMethodService::getAllPaymentMethods()
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payment_methods"];

    std::vector<PaymentMethod> paymentMethods;
    auto cursor = collection.find({});
    for (const auto &doc : cursor)
    {
        auto view = doc;
        paymentMethods.emplace_back(
            view["_id"].get_oid().value.to_string(),
            std::string(view["name"].get_string().value),
            std::string(view["description"].get_string().value));
    }
    return paymentMethods;
}

bool PaymentMethodService::updatePaymentMethod(const std::string &id, const PaymentMethod &paymentMethod)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payment_methods"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    bsoncxx::builder::basic::document set_doc{};
    set_doc.append(bsoncxx::builder::basic::kvp("name", paymentMethod.getName()));
    set_doc.append(bsoncxx::builder::basic::kvp("description", paymentMethod.getDescription()));

    bsoncxx::builder::basic::document update_doc{};
    update_doc.append(bsoncxx::builder::basic::kvp("$set", set_doc.view()));

    auto result = collection.update_one(query.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

bool PaymentMethodService::deletePaymentMethod(const std::string &id)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["payment_methods"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    auto result = collection.delete_one(query.view());
    return result && result->deleted_count() > 0;
}
