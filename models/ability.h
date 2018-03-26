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
#include <vector>

//enum class mark_player
//{
//    SELF,
//    OPPONENT,
//    ALL_OPPONENTS,
//    ALL_PLAYERS
//};

//enum class mark_card
//{
//    SELF,
//    ONE_YOU_CONTROL,
//    ONE_OPPONENT_CONTROLS,
//    ONE_OPPONENTS_CONTROL,
//    ALL_YOU_CONTROL,
//    ALL_OPPONENT_CONTROLS,
//    ALL
//};

//struct mark
//{
//    std::vector<mark_player> players;
//    std::vector<std::pair<mark_card, std::string>> cards; // pairs of quantity and kind, e.g.: all artifacts, one creature you control
//};

struct linearity
{
    int strength;
    std::string with;
};

struct static_ability
{
    std::string name;
    std::vector<linearity> linear_with;
//    bool is_mana_ability;
//    std::string marks; // target card, target player, whatever. Or, _all_ whatever. default is self
//    std::vector<std::pair<std::string, linearity>> linearities; // other abilities, card types, or specific cards, and the strength (linear, modular, antilinear)
};

// like a static ability, but has a cost to pay first
//struct activated_ability : public static_ability
//{
//    std::string cost;
//};


// like a static ability, but triggered by a particular condition
//struct triggered_ability : public static_ability
//{
//    std::string trigger;
//};