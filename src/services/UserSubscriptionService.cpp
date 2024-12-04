#include "UserSubscriptionService.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <mongocxx/exception/exception.hpp>
#include <iostream>

UserSubscriptionService::UserSubscriptionService(MongoConnection &connection) : _connection(connection) {}

std::string UserSubscriptionService::createSubscription(const UserSubscription &subscription)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["user_subscriptions"];

    bsoncxx::builder::basic::document document{};
    document.append(bsoncxx::builder::basic::kvp("userId", subscription.getUserId()));
    document.append(bsoncxx::builder::basic::kvp("planId", subscription.getPlanId()));
    document.append(bsoncxx::builder::basic::kvp("startDate", subscription.getStartDate()));
    document.append(bsoncxx::builder::basic::kvp("endDate", subscription.getEndDate()));
    document.append(bsoncxx::builder::basic::kvp("status", subscription.getStatus()));
    document.append(bsoncxx::builder::basic::kvp("isActive", subscription.isActive()));

    auto result = collection.insert_one(document.view());
    return result->inserted_id().get_oid().value.to_string();
}

std::optional<UserSubscription> UserSubscriptionService::getSubscriptionById(const std::string &id)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["user_subscriptions"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    auto doc = collection.find_one(query.view());
    if (doc)
    {
        auto view = doc->view();
        return UserSubscription(
            view["_id"].get_oid().value.to_string(),
            std::string(view["userId"].get_string().value),
            std::string(view["planId"].get_string().value),
            std::string(view["startDate"].get_string().value),
            std::string(view["endDate"].get_string().value),
            std::string(view["status"].get_string().value),
            view["isActive"].get_bool().value);
    }
    return std::nullopt;
}

std::vector<UserSubscription> UserSubscriptionService::getAllSubscriptions()
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["user_subscriptions"];

    std::vector<UserSubscription> subscriptions;
    auto cursor = collection.find({});
    for (const auto &doc : cursor)
    {
        auto view = doc;
        subscriptions.emplace_back(
            view["_id"].get_oid().value.to_string(),
            std::string(view["userId"].get_string().value),
            std::string(view["planId"].get_string().value),
            std::string(view["startDate"].get_string().value),
            std::string(view["endDate"].get_string().value),
            std::string(view["status"].get_string().value),
            view["isActive"].get_bool().value);
    }
    return subscriptions;
}

bool UserSubscriptionService::updateSubscription(const std::string &id, const UserSubscription &subscription)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["user_subscriptions"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    bsoncxx::builder::basic::document set_doc{};
    set_doc.append(bsoncxx::builder::basic::kvp("userId", subscription.getUserId()));
    set_doc.append(bsoncxx::builder::basic::kvp("planId", subscription.getPlanId()));
    set_doc.append(bsoncxx::builder::basic::kvp("startDate", subscription.getStartDate()));
    set_doc.append(bsoncxx::builder::basic::kvp("endDate", subscription.getEndDate()));
    set_doc.append(bsoncxx::builder::basic::kvp("status", subscription.getStatus()));
    set_doc.append(bsoncxx::builder::basic::kvp("isActive", subscription.isActive()));

    bsoncxx::builder::basic::document update_doc{};
    update_doc.append(bsoncxx::builder::basic::kvp("$set", set_doc.view()));

    auto result = collection.update_one(query.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

bool UserSubscriptionService::deleteSubscription(const std::string &id)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["user_subscriptions"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    auto result = collection.delete_one(query.view());
    return result && result->deleted_count() > 0;
}
