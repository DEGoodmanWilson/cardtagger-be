#include <iostream>

#include <luna/luna.h>
#include <json.hpp>

//#include <bsoncxx/builder/stream/document.hpp>
//#include <bsoncxx/json.hpp>
//
//#include <mongocxx/client.hpp>
//#include <mongocxx/instance.hpp>

#include "magique/catalog.h"

void error_logger(luna::log_level level, const std::string &message)
{
    switch (level)
    {
        case luna::log_level::DEBUG:
//            std::cerr << "[  DEBUG] " << message << std::endl;
            break;
        case luna::log_level::INFO:
            std::cerr << "[   INFO] " << message << std::endl;
            break;
        case luna::log_level::WARNING:
            std::cerr << "[WARNING] " << message << std::endl;
            break;
        case luna::log_level::ERROR:
            std::cerr << "[  ERROR] " << message << std::endl;
            break;
        case luna::log_level::FATAL:
            std::cerr << "[  FATAL] " << message << std::endl;
            break;
    }
}

void access_logger(const luna::request &request, const luna::response &response)
{
    std::cout << request.ip_address << ": " << luna::to_string(request.method) << " [" << response.status_code << "] "
              << request.path << " " << request.http_version << " " << (request.headers.count("user-agent") ? request.headers.at("user-agent") : "[no user-agent]") << " { "
              << std::chrono::duration_cast<std::chrono::microseconds>(request.end - request.start).count() << "us } " << std::endl;
}


int main(int, char **)
{

    // determine which port to run on, default to 8080
    auto port = 8080;
    if (auto port_str = std::getenv("PORT"))
    {
        try
        {
            port = std::atoi(port_str);
        }
        catch (const std::exception &e)
        {
            error_logger(luna::log_level::FATAL, "Invalid port specified in env $PORT.");
            return 1;
        }
    }



    // load up the card catalog
    magique::catalog catalog{"data/catalog.json"};

    // add endpoint handlers
    luna::router api{"/api/v1/card/"};

    api.set_mime_type("application/json");

    api.handle_request(luna::request_method::GET, std::regex{R"(([0-9]+))"}, [&](const auto &request) -> luna::response
    {
        auto id =  std::atoll(request.matches[1].c_str());
        nlohmann::json j;
        try
        {
            j = catalog.at(id);
        }
        catch (std::exception e)
        {
            return 404;
        }

        return j.dump();
    });

    // fire up the webserver
    luna::server server;
    server.start(port);

    return 0;
//    mongocxx::instance inst{};
//    mongocxx::client conn{mongocxx::uri{}};
//
//    bsoncxx::builder::stream::document document{};
//
//    auto collection = conn["testdb"]["testcollection"];
//    document << "hello" << "world";
//
//    collection.insert_one(document.view());
//    auto cursor = collection.find({});
//
//    for (auto&& doc : cursor) {
//        std::cout << bsoncxx::to_json(doc) << std::endl;
//    }
}