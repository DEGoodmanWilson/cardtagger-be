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

#include "card_delta.h"

namespace magique
{

void to_json(nlohmann::json &j, const card_delta &p)
{
    j = nlohmann::json::object();

    j["user_id"] = p.user_id;

    j["additions"] = nlohmann::json::object();
    j["subtractions"] = nlohmann::json::object();

    j["additions"]["abilities"] = p.additions.abilities;
    j["subtractions"]["abilities"] = p.subtractions.abilities;
}

void from_json(const nlohmann::json &j, card_delta &p)
{
    p.user_id = j.at("user_id").get<std::string>();

    p.additions.abilities = j.at("additions").at("abilities").get<std::set<std::string>>();
    p.subtractions.abilities = j.at("subtractions").at("abilities").get<std::set<std::string>>();
}

}