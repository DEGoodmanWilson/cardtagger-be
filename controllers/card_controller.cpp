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
#include <unordered_set>
#include <cpr/cpr.h>
#include <luna/luna.h>
#include <nlohmann/json.hpp>
#include <mongocxx/client.hpp>
#include <bsoncxx/json.hpp>
#include "card_controller.h"

namespace card_controller
{

static mongocxx::client mongo_conn_;
static mongocxx::collection card_annotations_;

std::unordered_set<std::string> validated_names_;

bool authenticate_(const luna::request &request)
{
    // TODO REFACTOR THIS SHIT, and also implement a proper auth microservice?
    // can we auth with scryfall?

    return true;

    // requires a header with a valid session key
//    if (request.headers.count("key") == 0)
//        return false;

//    return auth::test(request.headers.at("key"));
}

luna::response add_abilities_vote(const luna::request &request)
{
    // requires authentication
    if (!authenticate_(request))
    {
        return 401;
    }

    // TODO validate card name!
    std::string card_name = request.matches[1];
    if(!validated_names_.count(card_name))
    {
        //not in cache…check scryfall
        auto result = cpr::Get(cpr::Url{"https://api.scryfall.com/cards/named"}, cpr::Parameters{{"exact", card_name}});
        if(result.status_code==404)
            return {404, "Invalid card name \""+card_name+"\""};

        validated_names_.emplace(card_name);
    }

    // What we do is construct a vote for abilities this card exhibits. In the future, we will permit
    // adding additional information, like what those abilities target, how they are triggered,
    // and both manual and automatic interactions

    // expect an array of abilities as strings in the message body:
    //
    // ["Ability 1", "Ability 2"]
    // to appear in a listing, at least 75% of voters must have indicated a particular ability.
    //
    // We accept write-ins, too. We don't validate. For now.
    nlohmann::json abilities;
    try
    {
        abilities = nlohmann::json::parse(request.body);
    }
    catch(const nlohmann::json::parse_error &e)
    {
        return {400, "Invalid JSON. Please list abilities in a JSON array of strings in the request body."};
    }

    // validate that it's an array
    if (!abilities.is_array())
    {
        return {400, "Please list abilities in a JSON array of strings in the request body."};
    }

    for (const auto &ability: abilities)
    {
        if (!ability.is_string())
        {
            return {400, "Please list abilities in a JSON array of strings in the request body."};
        }
    }

    auto record = nlohmann::json::object();

    record["user"] = "test"; // TODO
    record["card"] = card_name;
    record["abilities"] = abilities;

    auto result = card_annotations_.insert_one(bsoncxx::from_json(record.dump()));

    if(!result)
    {
        return {500, "Failed to insert record"};
    }

    return {R"({"result": "ok"})"};
}


void create(std::shared_ptr<luna::router> router)
{
    auto my_uri = std::getenv("MONGO_URI");

    if (!my_uri)
    {
        std::cerr << "Invalid url specified in env MONGO_URI." << std::endl;
        exit(1); // TODO should throw an exception.
    }

    mongo_conn_ = mongocxx::uri{my_uri};
    card_annotations_ = mongo_conn_["magique"]["card_annotations"];

    router->handle_request(luna::request_method::POST,
                           std::regex{R"(/cards/(.+))"},
                           add_abilities_vote);
}


}