#include "MongoConnection.h"

MongoConnection::MongoConnection(const std::string &uri)
    : _client(mongocxx::uri{uri}) {}

mongocxx::database MongoConnection::getDatabase(const std::string &dbName)
{
    return _client[dbName];
}
