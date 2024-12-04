#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "config/AppConfig.h"
#include "database/MongoConnection.h"
#include "services/UserService.h"
#include "controllers/UserController.h"
#include "services/PlanService.h"
#include "controllers/PlanController.h"
#include "services/PaymentMethodService.h"
#include "controllers/PaymentMethodController.h"
#include "services/PaymentService.h"
#include "controllers/PaymentController.h"
#include "services/UserSubscriptionService.h"
#include "controllers/UserSubscriptionController.h"

using namespace web;
using namespace http;
using namespace http::experimental::listener;

// Clase para registrar las rutas
class ApiRouter
{
public:
    ApiRouter(http_listener &listener,
              UserController &userController,
              PlanController &planController,
              PaymentMethodController &paymentMethodController,
              PaymentController &paymentController,
              UserSubscriptionController &subscriptionController)
        : _listener(listener),
          _userController(userController),
          _planController(planController),
          _paymentMethodController(paymentMethodController),
          _paymentController(paymentController),
          _subscriptionController(subscriptionController) {}

    // cargar metodos
    void registerRoutes()
    {
        registerDefaultRoutes();
        registerUserRoutes();
        registerPlanRoutes();
        registerPaymentMethodRoutes();
        registerPaymentRoutes();
        registerSubscriptionRoutes();
    }

private:
    http_listener &_listener;
    UserController &_userController;
    PlanController &_planController;
    PaymentMethodController &_paymentMethodController;
    PaymentController &_paymentController;
    UserSubscriptionController &_subscriptionController;

    // Método para registrar rutas de usuarios
    void registerUserRoutes()
    {
        _listener.support(methods::POST, std::bind(&ApiRouter::handleUserPost, this, std::placeholders::_1));
        _listener.support(methods::GET, std::bind(&ApiRouter::handleUserGet, this, std::placeholders::_1));
        _listener.support(methods::PUT, std::bind(&ApiRouter::handleUserPut, this, std::placeholders::_1));
        _listener.support(methods::DEL, std::bind(&ApiRouter::handleUserDelete, this, std::placeholders::_1));
    }

    // Método para registrar rutas de planes
    void registerPlanRoutes()
    {
        _listener.support(methods::POST, std::bind(&ApiRouter::handlePlanPost, this, std::placeholders::_1));
        _listener.support(methods::GET, std::bind(&ApiRouter::handlePlanGet, this, std::placeholders::_1));
    }

    // Método para registrar rutas por defecto
    void registerDefaultRoutes()
    {
        _listener.support(methods::GET, [](http_request request)
                          {
            if (request.relative_uri().path() == "/") {
                json::value response;
                response["message"] = json::value::string("Welcome to the API!");
                request.reply(status_codes::OK, response);
            } else {
                request.reply(status_codes::NotFound, "Route not found.");
            } });
    }

    // Método para registrar rutas de métodos de pago
    void registerPaymentMethodRoutes()
    {
        _listener.support(methods::POST, std::bind(&ApiRouter::handlePaymentMethodPost, this, std::placeholders::_1));
        _listener.support(methods::GET, std::bind(&ApiRouter::handlePaymentMethodGet, this, std::placeholders::_1));
        _listener.support(methods::PUT, std::bind(&ApiRouter::handlePaymentMethodPut, this, std::placeholders::_1));
        _listener.support(methods::DEL, std::bind(&ApiRouter::handlePaymentMethodDelete, this, std::placeholders::_1));
    }

    // Método para registrar rutas de pagos
    void registerPaymentRoutes()
    {
        _listener.support(methods::POST, std::bind(&ApiRouter::handlePaymentPost, this, std::placeholders::_1));
        _listener.support(methods::GET, std::bind(&ApiRouter::handlePaymentGet, this, std::placeholders::_1));
        _listener.support(methods::PUT, std::bind(&ApiRouter::handlePaymentPut, this, std::placeholders::_1));
        _listener.support(methods::DEL, std::bind(&ApiRouter::handlePaymentDelete, this, std::placeholders::_1));
    }

    void registerSubscriptionRoutes()
    {
        // CRUD para UserSubscription
        _listener.support(methods::POST, std::bind(&ApiRouter::handleSubscriptionPost, this, std::placeholders::_1));
        _listener.support(methods::GET, std::bind(&ApiRouter::handleSubscriptionGet, this, std::placeholders::_1));
        _listener.support(methods::PUT, std::bind(&ApiRouter::handleSubscriptionPut, this, std::placeholders::_1));
        _listener.support(methods::DEL, std::bind(&ApiRouter::handleSubscriptionDelete, this, std::placeholders::_1));
    }

    /*
    |-----------------------------------------------------------------
    |           MANEJAR RUTAS Y EVENTOS
    |-----------------------------------------------------------------
    */

    // Métodos para manejar rutas de usuarios
    void handleUserPost(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "users")
        {
            request.extract_json().then([this, request](json::value body)
                                        {
                auto user = User("", body["name"].as_string(), body["email"].as_string(), body["password"].as_string());
                auto id = _userController.handleCreateUser(user);
                json::value response;
                response["id"] = json::value::string(id);
                request.reply(status_codes::OK, response); });
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handleUserGet(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "users")
        {
            if (paths.size() == 2)
            {
                auto id = paths[1];
                auto user = _userController.handleGetUserById(id);
                if (user)
                {
                    json::value response;
                    response["id"] = json::value::string(user->getId());
                    response["name"] = json::value::string(user->getName());
                    response["email"] = json::value::string(user->getEmail());
                    request.reply(status_codes::OK, response);
                }
                else
                {
                    request.reply(status_codes::NotFound);
                }
            }
            else
            {
                auto users = _userController.handleGetAllUsers();
                json::value response = json::value::array();
                int i = 0;
                for (const auto &user : users)
                {
                    json::value obj;
                    obj["id"] = json::value::string(user.getId());
                    obj["name"] = json::value::string(user.getName());
                    obj["email"] = json::value::string(user.getEmail());
                    response[i++] = obj;
                }
                request.reply(status_codes::OK, response);
            }
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handleUserPut(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "users" && paths.size() == 2)
        {
            request.extract_json().then([this, request, id = paths[1]](json::value body)
                                        {
                auto user = User(id, body["name"].as_string(), body["email"].as_string(), body["password"].as_string());
                if (_userController.handleUpdateUser(id, user)) {
                    request.reply(status_codes::OK);
                } else {
                    request.reply(status_codes::NotFound);
                } });
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handleUserDelete(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "users" && paths.size() == 2)
        {
            auto id = paths[1];
            if (_userController.handleDeleteUser(id))
            {
                request.reply(status_codes::OK);
            }
            else
            {
                request.reply(status_codes::NotFound);
            }
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    // Métodos para manejar rutas de planes
    void handlePlanPost(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "plans")
        {
            request.extract_json().then([this, request](json::value body)
                                        {
                auto plan = Plan("", body["name"].as_string(), body["price"].as_double(), body["durationDays"].as_integer());
                auto id = _planController.handleCreatePlan(plan);
                json::value response;
                response["id"] = json::value::string(id.value_or(""));
                request.reply(status_codes::OK, response); });
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handlePlanGet(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "plans")
        {
            auto plans = _planController.handleGetAllPlans();
            json::value response = json::value::array();
            int i = 0;
            for (const auto &plan : plans)
            {
                json::value obj;
                obj["id"] = json::value::string(plan.getId());
                obj["name"] = json::value::string(plan.getName());
                obj["price"] = plan.getPrice();
                obj["durationDays"] = plan.getDurationDays();
                response[i++] = obj;
            }
            request.reply(status_codes::OK, response);
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    // CRUD para Payment Methods
    void handlePaymentMethodPost(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "payment-methods")
        {
            request.extract_json().then([this, request](json::value body)
                                        {
                try {
                    auto paymentMethod = PaymentMethod("", body["name"].as_string(), body["description"].as_string());
                    auto id = _paymentMethodController.handleCreatePaymentMethod(paymentMethod);
                    json::value response;
                    response["id"] = json::value::string(id.value_or(""));
                    request.reply(status_codes::OK, response);
                } catch (const std::exception& e) {
                    request.reply(status_codes::BadRequest, e.what());
                } });
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handlePaymentMethodGet(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "payment-methods")
        {
            if (paths.size() == 2)
            {
                auto id = paths[1];
                auto paymentMethod = _paymentMethodController.handleGetPaymentMethodById(id);
                if (paymentMethod)
                {
                    json::value response;
                    response["id"] = json::value::string(paymentMethod->getId());
                    response["name"] = json::value::string(paymentMethod->getName());
                    response["description"] = json::value::string(paymentMethod->getDescription());
                    request.reply(status_codes::OK, response);
                }
                else
                {
                    request.reply(status_codes::NotFound);
                }
            }
            else
            {
                auto paymentMethods = _paymentMethodController.handleGetAllPaymentMethods();
                json::value response = json::value::array();
                int i = 0;
                for (const auto &method : paymentMethods)
                {
                    json::value obj;
                    obj["id"] = json::value::string(method.getId());
                    obj["name"] = json::value::string(method.getName());
                    obj["description"] = json::value::string(method.getDescription());
                    response[i++] = obj;
                }
                request.reply(status_codes::OK, response);
            }
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handlePaymentMethodPut(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "payment-methods" && paths.size() == 2)
        {
            request.extract_json().then([this, request, id = paths[1]](json::value body)
                                        {
                try {
                    auto paymentMethod = PaymentMethod(id, body["name"].as_string(), body["description"].as_string());
                    if (_paymentMethodController.handleUpdatePaymentMethod(id, paymentMethod)) {
                        request.reply(status_codes::OK);
                    } else {
                        request.reply(status_codes::NotFound);
                    }
                } catch (const std::exception& e) {
                    request.reply(status_codes::BadRequest, e.what());
                } });
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handlePaymentMethodDelete(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "payment-methods" && paths.size() == 2)
        {
            auto id = paths[1];
            if (_paymentMethodController.handleDeletePaymentMethod(id))
            {
                request.reply(status_codes::OK);
            }
            else
            {
                request.reply(status_codes::NotFound);
            }
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    // CRUD para Payments
    void handlePaymentPost(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "payments")
        {
            request.extract_json().then([this, request](json::value body)
                                        {
                try {
                    auto payment = Payment(
                        "",
                        body["userId"].as_string(),
                        body["paymentMethodId"].as_string(),
                        body["amount"].as_double(),
                        body["date"].as_string(),
                        body["status"].as_string()
                    );
                    auto id = _paymentController.handleCreatePayment(payment);
                    json::value response;
                    response["id"] = json::value::string(id.value_or(""));
                    request.reply(status_codes::OK, response);
                } catch (const std::exception& e) {
                    request.reply(status_codes::BadRequest, e.what());
                } });
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handlePaymentGet(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "payments")
        {
            if (paths.size() == 2)
            {
                auto id = paths[1];
                auto payment = _paymentController.handleGetPaymentById(id);
                if (payment)
                {
                    json::value response;
                    response["id"] = json::value::string(payment->getId());
                    response["userId"] = json::value::string(payment->getUserId());
                    response["paymentMethodId"] = json::value::string(payment->getPaymentMethodId());
                    response["amount"] = payment->getAmount();
                    response["date"] = json::value::string(payment->getDate());
                    response["status"] = json::value::string(payment->getStatus());
                    request.reply(status_codes::OK, response);
                }
                else
                {
                    request.reply(status_codes::NotFound);
                }
            }
            else
            {
                auto payments = _paymentController.handleGetAllPayments();
                json::value response = json::value::array();
                int i = 0;
                for (const auto &payment : payments)
                {
                    json::value obj;
                    obj["id"] = json::value::string(payment.getId());
                    obj["userId"] = json::value::string(payment.getUserId());
                    obj["paymentMethodId"] = json::value::string(payment.getPaymentMethodId());
                    obj["amount"] = payment.getAmount();
                    obj["date"] = json::value::string(payment.getDate());
                    obj["status"] = json::value::string(payment.getStatus());
                    response[i++] = obj;
                }
                request.reply(status_codes::OK, response);
            }
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handlePaymentPut(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "payments" && paths.size() == 2)
        {
            request.extract_json().then([this, request, id = paths[1]](json::value body)
                                        {
                try {
                    auto payment = Payment(
                        id,
                        body["userId"].as_string(),
                        body["paymentMethodId"].as_string(),
                        body["amount"].as_double(),
                        body["date"].as_string(),
                        body["status"].as_string()
                    );
                    if (_paymentController.handleUpdatePayment(id, payment)) {
                        request.reply(status_codes::OK);
                    } else {
                        request.reply(status_codes::NotFound);
                    }
                } catch (const std::exception& e) {
                    request.reply(status_codes::BadRequest, e.what());
                } });
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handlePaymentDelete(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "payments" && paths.size() == 2)
        {
            auto id = paths[1];
            if (_paymentController.handleDeletePayment(id))
            {
                request.reply(status_codes::OK);
            }
            else
            {
                request.reply(status_codes::NotFound);
            }
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    // Métodos CRUD para UserSubscription
    void handleSubscriptionPost(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "subscriptions")
        {
            request.extract_json().then([this, request](json::value body)
                                        {
                try {
                    auto subscription = UserSubscription(
                        "",
                        body["userId"].as_string(),
                        body["planId"].as_string(),
                        body["startDate"].as_string(),
                        body["endDate"].as_string(),
                        body["status"].as_string(),
                        body["isActive"].as_bool()
                    );
                    auto id = _subscriptionController.handleCreateSubscription(subscription);
                    json::value response;
                    response["id"] = json::value::string(id.value_or(""));
                    request.reply(status_codes::OK, response);
                } catch (const std::exception& e) {
                    request.reply(status_codes::BadRequest, e.what());
                } });
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handleSubscriptionGet(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "subscriptions")
        {
            if (paths.size() == 2)
            {
                auto id = paths[1];
                auto subscription = _subscriptionController.handleGetSubscriptionById(id);
                if (subscription)
                {
                    json::value response;
                    response["id"] = json::value::string(subscription->getId());
                    response["userId"] = json::value::string(subscription->getUserId());
                    response["planId"] = json::value::string(subscription->getPlanId());
                    response["startDate"] = json::value::string(subscription->getStartDate());
                    response["endDate"] = json::value::string(subscription->getEndDate());
                    response["status"] = json::value::string(subscription->getStatus());
                    response["isActive"] = subscription->isActive();
                    request.reply(status_codes::OK, response);
                }
                else
                {
                    request.reply(status_codes::NotFound);
                }
            }
            else
            {
                auto subscriptions = _subscriptionController.handleGetAllSubscriptions();
                json::value response = json::value::array();
                int i = 0;
                for (const auto &sub : subscriptions)
                {
                    json::value obj;
                    obj["id"] = json::value::string(sub.getId());
                    obj["userId"] = json::value::string(sub.getUserId());
                    obj["planId"] = json::value::string(sub.getPlanId());
                    obj["startDate"] = json::value::string(sub.getStartDate());
                    obj["endDate"] = json::value::string(sub.getEndDate());
                    obj["status"] = json::value::string(sub.getStatus());
                    obj["isActive"] = sub.isActive();
                    response[i++] = obj;
                }
                request.reply(status_codes::OK, response);
            }
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handleSubscriptionPut(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "subscriptions" && paths.size() == 2)
        {
            request.extract_json().then([this, request, id = paths[1]](json::value body)
                                        {
                try {
                    auto subscription = UserSubscription(
                        id,
                        body["userId"].as_string(),
                        body["planId"].as_string(),
                        body["startDate"].as_string(),
                        body["endDate"].as_string(),
                        body["status"].as_string(),
                        body["isActive"].as_bool()
                    );
                    if (_subscriptionController.handleUpdateSubscription(id, subscription)) {
                        request.reply(status_codes::OK);
                    } else {
                        request.reply(status_codes::NotFound);
                    }
                } catch (const std::exception& e) {
                    request.reply(status_codes::BadRequest, e.what());
                } });
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }

    void handleSubscriptionDelete(http_request request)
    {
        auto paths = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!paths.empty() && paths[0] == "subscriptions" && paths.size() == 2)
        {
            auto id = paths[1];
            if (_subscriptionController.handleDeleteSubscription(id))
            {
                request.reply(status_codes::OK);
            }
            else
            {
                request.reply(status_codes::NotFound);
            }
        }
        else
        {
            request.reply(status_codes::NotFound);
        }
    }
};

int main()
{
    try
    {
        MongoConnection connection(AppConfig::MONGODB_URI);

        // Configuración de controladores y servicios
        // User
        UserService userService(connection);
        UserController userController(userService);

        // Plan
        PlanService planService(connection);
        PlanController planController(planService);

        // Payment
        PaymentMethodService paymentMethodService(connection);
        PaymentMethodController paymentMethodController(paymentMethodService);

        // Payment Methos
        PaymentService paymentService(connection);
        PaymentController paymentController(paymentService);

        // user Subscription
        UserSubscriptionService subscriptionService(connection);
        UserSubscriptionController subscriptionController(subscriptionService);

        // Configurar servidor
        std::string address = "http://localhost:" + std::to_string(AppConfig::SERVER_PORT);
        http_listener listener(address);

        // Registrar rutas
        ApiRouter router(listener, userController, planController, paymentMethodController, paymentController, subscriptionController);
        router.registerRoutes();

        // Iniciar servidor
        listener
            .open()
            .then([address]()
                  { std::cout << "Server is running at: " << address << std::endl; })
            .wait();

        std::cout << "Press CTRL+C to exit." << std::endl;
        std::cin.get(); // Esperar que el usuario detenga el servidor
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
