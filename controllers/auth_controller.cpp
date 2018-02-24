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

#include "auth_controller.h"
#include "../models/auth.h"

namespace auth_controller
{

luna::response create_account(const luna::request &request);
luna::response delete_account(const luna::request &request);
luna::response login(const luna::request &request);
luna::response test(const luna::request &request);
luna::response logout(const luna::request &request);


void create(luna::router &router)
{
    // THIS IS TERRIBLE, do better
    router.handle_request(luna::request_method::POST, "/user/create", create_account, {{"username", true}, {"password", true}});
//    router.handle_request(luna::request_method::DELETE, "/user", delete_account, {{"key", true}});
    router.handle_request(luna::request_method::GET, "/user/login", login);
//    router.handle_request(luna::request_method::GET, "/user/test", test, {{"key", true}});
    router.handle_request(luna::request_method::GET, "/user/logout", logout);
}


luna::response create_account(const luna::request &request)
{
    auto result = auth::create_account(request.params.at("username"), request.params.at("password"));
    if(!result)
        return 409;
    return {R"({"success": true})"};
}

luna::response delete_account(const luna::request &request)
{
    auto result = auth::delete_account(request.headers.at("key"));
    if(!result)
        return 401;
    return {R"({"success": true})"};
}

luna::response login(const luna::request &request)
{
    auto result = auth::login(request.headers.at("username"), request.headers.at("password"));
    if(!result)
        return 401;
    return {R"({"success": true, "key": ")" + *result + "\"}"};
}

luna::response test(const luna::request &request)
{
    auto result = auth::test(request.headers.at("key"));
    if(!result)
        return 401;
    return {R"({"success": true})"};
}

luna::response logout(const luna::request &request)
{
    auth::logout(request.headers.at("key"));
    return {R"({"success": true})"};
}

}