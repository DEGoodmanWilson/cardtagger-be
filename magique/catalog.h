//
// Created by Don Goodman-Wilson on 13/11/2017.
//

#pragma once

#include <string>
#include <unordered_map>
#include "card.h"

namespace magique
{

class catalog
{
public:
    catalog(std::string filename, std::string annotations_filename="");

    const card &at(std::string name) const;
    const card &at(uint64_t name) const;

private:
    std::unordered_map<std::string, card> cards_by_name_;
    std::unordered_map<uint64_t, card> cards_by_id_;
};

}