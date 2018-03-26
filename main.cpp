//
//      ____________________________
//      \____  \____  \____  \____  \_
//       /   ___/  _/  /   /__/ __/  /
//      /   /__/   /  /   /  / /_/  /
//     /______/___   /___/__/___   /_____________________________
//               /__/  \___    /__/_ \____  \____  \____  \____  \_
//                      /   ___/  _/  /   /__/   /__/   ___/   /__/
//                     /   /  /   /  /   /__    /__    ___/   /  /
//                    /___/  /___   /___   /___   /___   /___/__/
//                              /__/   /__/   /__/   /__/   /
//
// cardtagger
// A web application for classifying collectible playing cards
//
// Copyright © 2018 D.E. Goodman-Wilson
//

#include <iostream>

#include <luna/luna.h>
#include <nlohmann/json.hpp>

#include "magique/catalog.h"
#include "controllers/auth_controller.h"
#include "controllers/card_controller.h"

void error_logger(luna::log_level level, const std::string &message)
{
    switch (level)
    {
        case luna::log_level::DEBUG:
            std::cerr << "[  DEBUG] " << message << std::endl;
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
              << request.path << " " << request.http_version << " "
              << (request.headers.count("user-agent") ? request.headers.at("user-agent") : "[no user-agent]") << " { "
              << std::chrono::duration_cast<std::chrono::microseconds>(request.end - request.start).count() << "us } "
              << std::endl;
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
            error_logger(luna::log_level::FATAL, "Invalid port specified in env PORT.");
            return 1;
        }
    }

    luna::server server;

    // add endpoint handlers
    auto api = server.create_router("/api/v1");

    api->set_mime_type("application/json");

    auth_controller::create(api);
    card_controller card_c(api);

    // fire up the webserver
    luna::set_error_logger(error_logger);
    luna::set_access_logger(access_logger);
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