#pragma once
#include "../services/UserService.h"
#include <vector>
#include <optional>

class UserController {
public:
    UserController(UserService& service);

    std::string handleCreateUser(const User& user);
    std::optional<User> handleGetUserById(const std::string& id);
    std::vector<User> handleGetAllUsers();
    bool handleUpdateUser(const std::string& id, const User& user);
    bool handleDeleteUser(const std::string& id);
    std::optional<std::string> handleLogin(const std::string& email, const std::string& password);

private:
    UserService& _userService;
};
