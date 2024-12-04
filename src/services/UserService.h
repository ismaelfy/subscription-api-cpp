#pragma once
#include "../models/User.h"
#include "../database/MongoConnection.h"
#include <vector>
#include <optional>

class UserService {
public:
    UserService(MongoConnection& connection);

    std::string createUser(const User& user);
    std::optional<User> getUserById(const std::string& id);
    std::vector<User> getAllUsers();
    bool updateUser(const std::string& id, const User& user);
    bool deleteUser(const std::string& id);
    std::optional<std::string> login(const std::string& email, const std::string& password);

private:
    MongoConnection& _connection;
};
