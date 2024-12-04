#pragma once
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

class MongoConnection {
public:
    MongoConnection(const std::string& uri);
    mongocxx::database getDatabase(const std::string& dbName);

private:
    mongocxx::instance _instance;
    mongocxx::client _client;
};
