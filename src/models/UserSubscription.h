#pragma once
#include <string>

class UserSubscription
{
public:
    UserSubscription(const std::string &id, const std::string &userId, const std::string &planId,
                     const std::string &startDate, const std::string &endDate, const std::string &status, bool isActive)
        : _id(id), _userId(userId), _planId(planId), _startDate(startDate), _endDate(endDate), _status(status), _isActive(isActive) {}

    const std::string &getId() const { return _id; }
    const std::string &getUserId() const { return _userId; }
    const std::string &getPlanId() const { return _planId; }
    const std::string &getStartDate() const { return _startDate; }
    const std::string &getEndDate() const { return _endDate; }
    const std::string &getStatus() const { return _status; }
    bool isActive() const { return _isActive; }

    void setId(const std::string &id) { _id = id; }
    void setUserId(const std::string &userId) { _userId = userId; }
    void setPlanId(const std::string &planId) { _planId = planId; }
    void setStartDate(const std::string &startDate) { _startDate = startDate; }
    void setEndDate(const std::string &endDate) { _endDate = endDate; }
    void setStatus(const std::string &status) { _status = status; }
    void setIsActive(bool isActive) { _isActive = isActive; }

private:
    std::string _id;
    std::string _userId;
    std::string _planId;
    std::string _startDate;
    std::string _endDate;
    std::string _status;
    bool _isActive; // Renombrado de isTrial a isActive
};
