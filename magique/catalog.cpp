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

#include "catalog.h"

#include <exception>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

namespace magique
{

catalog::catalog() :
        mongo_inst_{}
{
    auto my_uri = std::getenv("MONGO_URI");

    if (!my_uri)
    {
        std::cerr << "Invalid url specified in env MONGO_URI." << std::endl;
        exit(1); // TODO should throw an exception.
    }

    mongo_conn_ = mongocxx::uri{my_uri};
    catalog_ = mongo_conn_["magique"]["catalog"];
}

std::string catalog::at(std::string name)
{
    auto result = catalog_.find_one(document{} << "name" << name << finalize);
    if (result)
    {
        auto json = bsoncxx::to_json(result->view());
        return json;
    }

    throw std::out_of_range{"Could not find " + name};
}

std::string catalog::random()
{
    auto result = catalog_.aggregate(mongocxx::pipeline{}.sample(1));
    auto json = bsoncxx::to_json(*result.begin());
    return json;
}
}