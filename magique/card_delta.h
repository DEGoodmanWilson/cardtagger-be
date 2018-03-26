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

#pragma once

#include <nlohmann/json.hpp>
#include <set>

namespace magique
{

struct card_delta
{
    std::string user_id;
    struct additions
    {
        std::set<std::string> abilities;
    } additions;
    struct subtractions
    {
        std::set<std::string> abilities;
    } subtractions;
};

void to_json(nlohmann::json &j, const card_delta &p);

void from_json(const nlohmann::json &j, card_delta &p);
}