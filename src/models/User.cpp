#include "User.h"

User::User(const std::string &id, const std::string &name, const std::string &email, const std::string &password)
    : _id(id), _name(name), _email(email), _password(password) {}

const std::string &User::getId() const { return _id; }
const std::string &User::getName() const { return _name; }
const std::string &User::getEmail() const { return _email; }
const std::string &User::getPassword() const { return _password; }
