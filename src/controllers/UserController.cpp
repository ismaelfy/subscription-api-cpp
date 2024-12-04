#include "UserController.h"

UserController::UserController(UserService& service) : _userService(service) {}

std::string UserController::handleCreateUser(const User& user) {
    return _userService.createUser(user);
}

std::optional<User> UserController::handleGetUserById(const std::string& id) {
    return _userService.getUserById(id);
}

std::vector<User> UserController::handleGetAllUsers() {
    return _userService.getAllUsers();
}

bool UserController::handleUpdateUser(const std::string& id, const User& user) {
    return _userService.updateUser(id, user);
}

bool UserController::handleDeleteUser(const std::string& id) {
    return _userService.deleteUser(id);
}

std::optional<std::string> UserController::handleLogin(const std::string& email, const std::string& password) {
    return _userService.login(email, password);
}
