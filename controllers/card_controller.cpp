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

#include "card_controller.h"
#include "../magique/card.h"
#include "../models/auth.h"


card_controller::card_controller(std::shared_ptr<luna::router> router)
{
    // THIS IS TERRIBLE, do better
    router->handle_request(luna::request_method::GET,
                          std::regex{R"(/card/(.+))"},
                          std::bind(&card_controller::get_card_by_name_, this, std::placeholders::_1));
    router->handle_request(luna::request_method::GET,
                          std::regex{R"(/card/?)"}, // TODO fix Luna, this is a hack.
                          std::bind(&card_controller::get_random_card_,
                                    this,
                                    std::placeholders::_1));
    router->handle_request(luna::request_method::PATCH,
                          std::regex{R"(/card/(.+))"},
                          std::bind(&card_controller::update_card_, this, std::placeholders::_1));
}

luna::response card_controller::get_card_by_name_(const luna::request &request)
{
    try
    {
        auto c_j = nlohmann::json::parse(catalog_.at(request.matches[1]));
        magique::card c = c_j.get<magique::card>();
        c_j = c; // filter out all the unwanted fields
        return c_j.dump();
    }
    catch (std::exception e)
    {
        luna::error_log(luna::log_level::DEBUG, e.what());
        return 404;
    }

    return 404;
}

luna::response card_controller::get_random_card_(const luna::request &request)
{
    auto c_j = nlohmann::json::parse(catalog_.random());
    magique::card c = c_j.get<magique::card>();
    c_j = c; // filter out all the unwanted fields
    return c_j.dump();
}

bool authenticate_(const luna::request &request)
{
    // requires a header with a valid session key
    if(request.headers.count("key") == 0)
        return false;

    return auth::test(request.headers.at("key"));
}

luna::response card_controller::update_card_(const luna::request &request)
{
    // requires authentication
    if(!authenticate_(request))
    {
        return 401;
    }

    magique::card c;

    try
    {
        auto c_j = nlohmann::json::parse(catalog_.at(request.matches[1]));
        c = c_j.get<magique::card>();
    }
    catch (std::exception e)
    {
        luna::error_log(luna::log_level::DEBUG, e.what());
        return 400;
    }
    
    try
    {
        auto updates_j = nlohmann::json::parse(request.body);
    }
    catch (std::exception e)
    {
        luna::error_log(luna::log_level::DEBUG, e.what());
        return 400;
    }
    // the changes will be json in the message body:
    // {
    //    "abilities": {
    //      "direct_damage": true, //add "direct_damage"
    //      "foobar": false // remove "foobar"
    //    }
    //    "affinities": {
    //    }
    //    "triggers": {
    //    }
    // }


    // TODO need to work out how this will work…clearlyt we don't want people just updating a card willy-nilly, we need to be methodical about it

    // First, we need a log of who has shared what updates.


    // Second, we need to be able to calculate based on some kind of average what a card's abilities look like in aggregate
    // Third, we need to show what the modifications the user has submitted.

    return {"UPDATE"};
}