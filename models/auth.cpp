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

#include "auth.h"

#include <unordered_map>

namespace auth
{
namespace private_
{

// TODO THIS all needs to be _persisted!_ not just stored in memory!

struct user
{
    std::string pass;
};

static std::unordered_map<std::string, user> users;
static std::unordered_map<std::string, std::string> sessions;

bool authed(const std::string &key)
{
    return private_::sessions.count(key) > 0;
}

}

bool create_account(const std::string &username, const std::string &password)
{
    if(private_::users.count(username) > 0)
        return false;

    private_::users[username] = {password}; // TODO hash it! Obvs.
    return true;
}

bool delete_account(const std::string &key)
{
    if(!private_::authed(key)) return false;
    std::string username = private_::sessions.at(key);
    logout(key);
    private_::users.erase(username);
    return true;
}


std::experimental::optional<std::string> login(const std::string &username, const std::string &password)
{
    if (!private_::users.count(username)) return std::experimental::nullopt;

    if (private_::users.at(username).pass != password) return std::experimental::nullopt;

    std::string key{username + ":" + password};
    private_::sessions[key] = username;

    return {key};
}

bool test(const std::string &key)
{
    return private_::authed(key);
}

void logout(const std::string &key)
{
    if(private_::authed(key))
    {
        private_::sessions.erase(key);
    }
}

}