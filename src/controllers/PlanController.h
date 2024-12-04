#pragma once
#include "../services/PlanService.h"
#include "../models/Plan.h"
#include <optional>

class PlanController
{
public:
    PlanController(PlanService &service);

    std::optional<std::string> handleCreatePlan(const Plan &plan);
    std::optional<Plan> handleGetPlanById(const std::string &id);
    std::vector<Plan> handleGetAllPlans();
    bool handleUpdatePlan(const std::string &id, const Plan &plan);
    bool handleDeletePlan(const std::string &id);

private:
    PlanService &_planService;
};
