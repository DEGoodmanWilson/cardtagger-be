//
// Created by Don Goodman-Wilson on 13/11/2017.
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
    catalog(std::string url);

    std::string at(std::string name);

    std::string random();

private:
    std::unordered_map<std::string, card> cards_by_name_;

    mongocxx::instance mongo_inst_;
    mongocxx::client mongo_conn_;
    mongocxx::collection catalog_;
};

}