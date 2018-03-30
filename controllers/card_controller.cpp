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
#include <experimental/optional>
#include <cpr/cpr.h>
#include <luna/luna.h>
#include <nlohmann/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include "card_controller.h"

class card_controller::card_controller_impl_
{
public:
    mongocxx::pool db_pool_;

    card_controller_impl_(const mongocxx::uri &uri) : db_pool_{uri} {}

    bool authenticate_(const luna::request &request);

    luna::response get_abilities_(const luna::request &request);
    luna::response add_abilities_vote_(const luna::request &request);

    struct card_name_hasher_ {
    public:
        size_t operator()(const std::string & str) const {
            std::string str_lower{str};
            std::transform(str_lower.begin(), str_lower.end(), str_lower.begin(), tolower);
            return std::hash<std::string>()(str_lower);
        }
    };

    struct card_name_comparator_ {
    public:
        bool operator()(const std::string & str1, const std::string & str2) const {
            std::string str1_lower{str1};
            std::string str2_lower{str2};
            std::transform(str1_lower.begin(), str1_lower.end(), str1_lower.begin(), tolower);
            std::transform(str2_lower.begin(), str2_lower.end(), str2_lower.begin(), tolower);
            return str1_lower == str2_lower;
        }
    };

private:
    std::experimental::optional<std::string> validate_card_name_(std::string card_name);
    std::unordered_set<std::string, card_name_hasher_, card_name_comparator_> validated_names_;
};

bool card_controller::card_controller_impl_::authenticate_(const luna::request &request)
{
    // TODO REFACTOR THIS SHIT, and also implement a proper auth microservice?
    // can we auth with scryfall?

    return true;

    // requires a header with a valid session key
//    if (request.headers.count("key") == 0)
//        return false;

//    return auth::test(request.headers.at("key"));
}

std::experimental::optional<std::string> card_controller::card_controller_impl_::validate_card_name_(std::string card_name)
{
    if(validated_names_.count(card_name) == 0)
    {
        //not in cache…check scryfall
        auto result = cpr::Get(cpr::Url{"https://api.scryfall.com/cards/named"}, cpr::Parameters{{"exact", card_name}});
        if(result.status_code==404)
            return std::experimental::nullopt;

        auto card_obj = nlohmann::json::parse(result.text);
        auto normalized_card_name = card_obj["name"].get<std::string>();

        // This is safer, as we are now cacheing card names in a case insensitive way
        // As there are only some 16,000 different legal Magic Card names, I think this is safe as is.
        validated_names_.insert(card_name);

        return card_name;
    }

    return *validated_names_.find(card_name);
}

luna::response card_controller::card_controller_impl_::get_abilities_(const luna::request &request)
{
    return {"[]"};
}

luna::response card_controller::card_controller_impl_::add_abilities_vote_(const luna::request &request)
{
    // requires authentication
    if (!authenticate_(request))
    {
        return 401;
    }


    auto validated_card_name = validate_card_name_(request.matches[1]);
    if(!validated_card_name)
        return {404, "Invalid card name \""+request.matches[1]+"\""};

    // Connect to DB
    auto client = db_pool_.acquire();
    auto card_annotations = (*client)["magique"]["card_annotations"];

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
    record["card"] = *validated_card_name;
    record["abilities"] = abilities;

    auto result = card_annotations.insert_one(bsoncxx::from_json(record.dump()));

    if(!result)
    {
        return {500, "Failed to insert record"};
    }

    return {R"({"result": "ok"})"};
}


card_controller::card_controller(std::shared_ptr<luna::router> router)
{
    auto my_uri = std::getenv("MONGO_URI");

    if (!my_uri)
    {
        std::cerr << "Invalid url specified in env MONGO_URI." << std::endl;
        exit(1); // TODO should throw an exception.
    }

    impl_ = std::make_shared<card_controller_impl_>(mongocxx::uri{my_uri});

    router->handle_request(luna::request_method::GET,
                           std::regex{R"(/cards/(.+))"},
                           std::bind(&card_controller::card_controller_impl_::get_abilities_, impl_.get(), std::placeholders::_1));

    router->handle_request(luna::request_method::POST,
                           std::regex{R"(/cards/(.+))"},
                           std::bind(&card_controller::card_controller_impl_::add_abilities_vote_, impl_.get(), std::placeholders::_1));
}