#pragma once
#include <string>

class AppConfig
{
public:
    static const std::string MONGODB_URI;
    static const int SERVER_PORT;
};

// Implementación de valores
const std::string AppConfig::MONGODB_URI = "mongodb+srv://a2023100730:RKz7JCu9x9XkaH7F@subscription.g9mfb.mongodb.net/subscription?retryWrites=true&w=majority&appName=Subscription";
const int AppConfig::SERVER_PORT = 4000;
