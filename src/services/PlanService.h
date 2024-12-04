#pragma once
#include "../models/Plan.h"
#include "../database/MongoConnection.h"
#include <vector>
#include <optional>

class PlanService
{
public:
    PlanService(MongoConnection &connection);

    std::string createPlan(const Plan &plan);
    std::optional<Plan> getPlanById(const std::string &id);
    std::vector<Plan> getAllPlans();
    bool updatePlan(const std::string &id, const Plan &plan);
    bool deletePlan(const std::string &id);

private:
    MongoConnection &_connection;
};
