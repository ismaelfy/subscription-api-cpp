#pragma once
#include "../models/UserSubscription.h"
#include "../database/MongoConnection.h"
#include <vector>
#include <optional>

class UserSubscriptionService
{
public:
    UserSubscriptionService(MongoConnection &connection);

    std::string createSubscription(const UserSubscription &subscription);
    std::optional<UserSubscription> getSubscriptionById(const std::string &id);
    std::vector<UserSubscription> getAllSubscriptions();
    bool updateSubscription(const std::string &id, const UserSubscription &subscription);
    bool deleteSubscription(const std::string &id);

private:
    MongoConnection &_connection;
};
