#include "UserSubscriptionController.h"

UserSubscriptionController::UserSubscriptionController(UserSubscriptionService &service) : _subscriptionService(service) {}

std::optional<std::string> UserSubscriptionController::handleCreateSubscription(const UserSubscription &subscription)
{
    return _subscriptionService.createSubscription(subscription);
}

std::optional<UserSubscription> UserSubscriptionController::handleGetSubscriptionById(const std::string &id)
{
    return _subscriptionService.getSubscriptionById(id);
}

std::vector<UserSubscription> UserSubscriptionController::handleGetAllSubscriptions()
{
    return _subscriptionService.getAllSubscriptions();
}

bool UserSubscriptionController::handleUpdateSubscription(const std::string &id, const UserSubscription &subscription)
{
    return _subscriptionService.updateSubscription(id, subscription);
}

bool UserSubscriptionController::handleDeleteSubscription(const std::string &id)
{
    return _subscriptionService.deleteSubscription(id);
}
