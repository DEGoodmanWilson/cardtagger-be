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
#include <luna/optional.hpp>

namespace auth
{

// Logging in returns a key that can be used for a limited time, and perhaps with other restrictions.

bool create_account(const std::string &username, const std::string &password);

bool delete_account(const std::string &key);

std::experimental::optional<std::string> login(const std::string &username, const std::string &password);

bool test(const std::string &key);

void logout(const std::string &key);

}