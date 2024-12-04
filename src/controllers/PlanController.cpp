#include "PlanController.h"

PlanController::PlanController(PlanService &service) : _planService(service) {}

std::optional<std::string> PlanController::handleCreatePlan(const Plan &plan)
{
    return _planService.createPlan(plan);
}

std::optional<Plan> PlanController::handleGetPlanById(const std::string &id)
{
    return _planService.getPlanById(id);
}

std::vector<Plan> PlanController::handleGetAllPlans()
{
    return _planService.getAllPlans();
}

bool PlanController::handleUpdatePlan(const std::string &id, const Plan &plan)
{
    return _planService.updatePlan(id, plan);
}

bool PlanController::handleDeletePlan(const std::string &id)
{
    return _planService.deletePlan(id);
}
