#include "PlanService.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/exception/exception.hpp>
#include <iostream>

using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;

PlanService::PlanService(MongoConnection &connection) : _connection(connection) {}

std::string PlanService::createPlan(const Plan &plan) {
    try {
        auto db = _connection.getDatabase("mydatabase");
        auto collection = db["plans"];

        document doc{};
        doc.append(kvp("name", plan.getName()));
        doc.append(kvp("price", plan.getPrice()));
        doc.append(kvp("durationDays", plan.getDurationDays()));

        auto result = collection.insert_one(doc.view());
        if (result) {
            return result->inserted_id().get_oid().value.to_string();
        }
    } catch (const mongocxx::exception &e) {
        std::cerr << "MongoDB Exception: " << e.what() << std::endl;
    }
    return {};
}

std::optional<Plan> PlanService::getPlanById(const std::string &id) {
    try {
        auto db = _connection.getDatabase("mydatabase");
        auto collection = db["plans"];

        document query{};
        query.append(kvp("_id", bsoncxx::oid(id)));

        auto doc = collection.find_one(query.view());
        if (doc) {
            auto view = doc->view();
            return Plan(
                view["_id"].get_oid().value.to_string(),
                std::string(view["name"].get_string().value),
                view["price"].get_double().value,
                view["durationDays"].get_int32().value);
        }
    } catch (const mongocxx::exception &e) {
        std::cerr << "MongoDB Exception: " << e.what() << std::endl;
    }
    return std::nullopt;
}

std::vector<Plan> PlanService::getAllPlans() {
    std::vector<Plan> plans;
    try {
        auto db = _connection.getDatabase("mydatabase");
        auto collection = db["plans"];

        auto cursor = collection.find({});
        for (const auto &doc : cursor) {
            auto view = doc;
            plans.emplace_back(
                view["_id"].get_oid().value.to_string(),
                std::string(view["name"].get_string().value),
                view["price"].get_double().value,
                view["durationDays"].get_int32().value);
        }
    } catch (const mongocxx::exception &e) {
        std::cerr << "MongoDB Exception: " << e.what() << std::endl;
    }
    return plans;
}

bool PlanService::updatePlan(const std::string &id, const Plan &plan) {
    try {
        auto db = _connection.getDatabase("mydatabase");
        auto collection = db["plans"];

        document query{};
        query.append(kvp("_id", bsoncxx::oid(id)));

        document update_doc{};
        document set_doc{};
        set_doc.append(kvp("name", plan.getName()));
        set_doc.append(kvp("price", plan.getPrice()));
        set_doc.append(kvp("durationDays", plan.getDurationDays()));

        update_doc.append(kvp("$set", set_doc));

        auto result = collection.update_one(query.view(), update_doc.view());
        return result && result->modified_count() > 0;
    } catch (const mongocxx::exception &e) {
        std::cerr << "MongoDB Exception: " << e.what() << std::endl;
    }
    return false;
}

bool PlanService::deletePlan(const std::string &id) {
    try {
        auto db = _connection.getDatabase("mydatabase");
        auto collection = db["plans"];

        document query{};
        query.append(kvp("_id", bsoncxx::oid(id)));

        auto result = collection.delete_one(query.view());
        return result && result->deleted_count() > 0;
    } catch (const mongocxx::exception &e) {
        std::cerr << "MongoDB Exception: " << e.what() << std::endl;
    }
    return false;
}
