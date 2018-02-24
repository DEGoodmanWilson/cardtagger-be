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

#include <string>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "card.h"

namespace magique
{

class catalog
{
public:
    catalog();

    std::string at(std::string name);

    std::string random();

private:
    std::unordered_map<std::string, card> cards_by_name_;

    mongocxx::instance mongo_inst_;
    mongocxx::client mongo_conn_;
    mongocxx::collection catalog_;
};

}