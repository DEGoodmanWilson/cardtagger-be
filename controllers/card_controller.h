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

#include <luna/luna.h>
#include <json.hpp>
#include "../magique/catalog.h"


class card_controller
{
public:
    card_controller(luna::router &router);

private:
    // TODO PIMPL this.
    luna::response get_card_by_name_(const luna::request &request);
    luna::response get_random_card_(const luna::request &request);
    luna::response update_card_(const luna::request &request);
    magique::catalog catalog_;
};