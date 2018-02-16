//
// Created by Don Goodman-Wilson on 13/11/2017.
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

catalog::catalog(std::string url) :
        mongo_inst_{}, mongo_conn_{mongocxx::uri{url}}, catalog_{mongo_conn_["magique"]["catalog"]}
{}

std::string catalog::at(std::string name)
{
    auto result = catalog_.find_one(document{} << "name" << name << finalize);
    if (result)
    {
        auto json = bsoncxx::to_json(result->view());
        std::cout << json << std::endl;
        return json;
    }

    throw std::out_of_range{"Could not find " + name};
}

std::string catalog::random()
{
    auto result = catalog_.aggregate(mongocxx::pipeline{}.sample(1));
    auto json = bsoncxx::to_json(*result.begin());
    std::cout << json << std::endl;
    return json;
}
}