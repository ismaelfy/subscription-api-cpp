#pragma once
#include "../services/UserSubscriptionService.h"
#include "../models/UserSubscription.h"
#include <optional>

class UserSubscriptionController
{
public:
    UserSubscriptionController(UserSubscriptionService &service);

    std::optional<std::string> handleCreateSubscription(const UserSubscription &subscription);
    std::optional<UserSubscription> handleGetSubscriptionById(const std::string &id);
    std::vector<UserSubscription> handleGetAllSubscriptions();
    bool handleUpdateSubscription(const std::string &id, const UserSubscription &subscription);
    bool handleDeleteSubscription(const std::string &id);

private:
    UserSubscriptionService &_subscriptionService;
};
