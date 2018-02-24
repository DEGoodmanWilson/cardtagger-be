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
#include <fstream>

#include <luna/luna.h>
#include <json.hpp>

#include "magique/card.h"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

int main(void)
{
    auto my_uri = std::getenv("MONGO_URI");

    if (!my_uri)
    {
        return 1;
    }

    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{my_uri}};

    auto collection = conn["magique"]["catalog"];

    std::ifstream ifs("data/catalog.json");
    nlohmann::json card_list_json{ifs};
    ifs.close();

    nlohmann::json annotations;
    ifs.open("data/annotations.json");
    annotations << ifs;

    //assume for now that it is just a vector of cards
    std::unordered_map<std::string, magique::card> catalog_memory;
    for (auto &card_json: card_list_json)
    {
        auto name = card_json["name"].get<std::string>();

        if (catalog_memory.count(name))
        {
            // already exists, just add the multiverseid
            try
            {
                catalog_memory[name].multiverseids.emplace_back(card_json["multiverseid"].get<uint64_t>());
            }
            catch (const std::exception &e)
            {}

            continue;
        }

        //load its annotations, if any
        try
        {
            const auto a = annotations[name];
            for (auto it = a.begin(); it != a.end(); ++it)
            {
                card_json[it.key()] = it.value();
            }
        }
        catch (const std::out_of_range &e)
        {}

        catalog_memory[name] = card_json;
    }

    std::vector<bsoncxx::document::value> catalog;
    for (const auto &kv : catalog_memory)
    {
        nlohmann::json card_json = std::get<magique::card>(kv);
        catalog.emplace_back(bsoncxx::from_json(card_json.dump()));
    }

    collection.insert_many(catalog);

//    document << "hello" << "world";
//
//    collection.insert_one(document.view());
//    auto cursor = collection.find({});
//
//    for (auto &&doc : cursor)
//    {
//        std::cout << bsoncxx::to_json(doc) << std::endl;
//    }

    return 0;
}