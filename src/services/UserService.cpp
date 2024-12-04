#include "UserService.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/exception/exception.hpp>
#include <iostream>

UserService::UserService(MongoConnection &connection) : _connection(connection) {}

std::string UserService::createUser(const User &user)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["users"];

    bsoncxx::builder::basic::document document{};
    document.append(bsoncxx::builder::basic::kvp("name", user.getName()));
    document.append(bsoncxx::builder::basic::kvp("email", user.getEmail()));
    document.append(bsoncxx::builder::basic::kvp("password", user.getPassword()));

    auto result = collection.insert_one(document.view());
    return result->inserted_id().get_oid().value.to_string();
}

std::optional<User> UserService::getUserById(const std::string &id)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["users"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    auto doc = collection.find_one(query.view());
    if (doc)
    {
        auto view = doc->view();
        return User(
            view["_id"].get_oid().value.to_string(),
            std::string(view["name"].get_string().value),
            std::string(view["email"].get_string().value),
            std::string(view["password"].get_string().value));
    }
    return std::nullopt;
}

std::vector<User> UserService::getAllUsers()
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["users"];

    std::vector<User> users;
    auto cursor = collection.find({});
    for (const auto &doc : cursor)
    {
        auto view = doc;
        users.emplace_back(
            view["_id"].get_oid().value.to_string(),
            std::string(view["name"].get_string().value),
            std::string(view["email"].get_string().value),
            std::string(view["password"].get_string().value));
    }
    return users;
}

bool UserService::updateUser(const std::string &id, const User &user)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["users"];

    // Crear la consulta para identificar el documento
    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    // Crear el documento de actualización
    bsoncxx::builder::basic::document set_doc{};
    set_doc.append(bsoncxx::builder::basic::kvp("name", user.getName()));
    set_doc.append(bsoncxx::builder::basic::kvp("email", user.getEmail()));
    set_doc.append(bsoncxx::builder::basic::kvp("password", user.getPassword()));

    bsoncxx::builder::basic::document update_doc{};
    update_doc.append(bsoncxx::builder::basic::kvp("$set", set_doc.view()));

    // Ejecutar la actualización
    auto result = collection.update_one(query.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

bool UserService::deleteUser(const std::string &id)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["users"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id)));

    auto result = collection.delete_one(query.view());
    return result && result->deleted_count() > 0;
}

std::optional<std::string> UserService::login(const std::string &email, const std::string &password)
{
    auto db = _connection.getDatabase("mydatabase");
    auto collection = db["users"];

    bsoncxx::builder::basic::document query{};
    query.append(bsoncxx::builder::basic::kvp("email", email));
    query.append(bsoncxx::builder::basic::kvp("password", password));

    auto doc = collection.find_one(query.view());
    if (doc)
    {
        return doc->view()["_id"].get_oid().value.to_string();
    }
    return std::nullopt;
}
