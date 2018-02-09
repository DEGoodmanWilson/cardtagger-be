//
// Created by Don Goodman-Wilson on 13/11/2017.
//

#include "catalog.h"
#include <json.hpp>
#include <fstream>

namespace magique
{

catalog::catalog(std::string filename)
{
    std::ifstream ifs(filename);
    nlohmann::json card_list_json{ifs};
    ifs.close();


    //assume for now that it is just a vector of cards
    for (auto &card_json: card_list_json)
    {
        auto id = card_json["multiverseid"].get<int>();
        cards_by_id_[id] = card_json;

        auto name = card_json["name"].get<std::string>();
        cards_by_name_[name] = card_json;
    }
}

const card &catalog::at(std::string name) const
{
    return cards_by_name_.at(name); //will throw std::out_of_range if not found
}

const card &catalog::at(uint64_t id) const
{
    return cards_by_id_.at(id); //will throw std::out_of_range if not found
}

void catalog::insert(const card &card)
{
    cards_by_id_[card.multiverseid] = card;
    cards_by_name_[card.name] = card;
}

void catalog::insert(::magique::card &&card)
{
    cards_by_id_.emplace(card.multiverseid, std::move(card));
    cards_by_name_.emplace(card.name, std::move(card));
}

}