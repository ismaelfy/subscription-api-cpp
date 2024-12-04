#pragma once
#include <string>

class User
{
public:
    User(const std::string &id, const std::string &name, const std::string &email, const std::string &password);

    const std::string &getId() const;
    const std::string &getName() const;
    const std::string &getEmail() const;
    const std::string &getPassword() const;

private:
    std::string _id;
    std::string _name;
    std::string _email;
    std::string _password;
};
