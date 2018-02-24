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


#include <catch.hpp>
#include <cpr/cpr.h>
#include <json.hpp>
#include "env.h"
#include "../models/auth.h"


//TEST_CASE("testing authentication")
//{
//
//    auto result = auth::create_account("user", "pass");
//    REQUIRE(result);
//
//    SECTION("auth login basic functioning")
//    {
//        auto key = auth::login("user", "pass");
//        REQUIRE(static_cast<bool>(key));
//        auto result = auth::test(*key);
//        REQUIRE(result);
//    }
//
//    SECTION("auth logout basic functioning")
//    {
//        auto key = auth::login("user", "pass");
//        REQUIRE(static_cast<bool>(key));
//        auto result = auth::test(*key);
//        REQUIRE(result);
//        auth::logout(*key);
//        result = auth::test(*key);
//        REQUIRE(!result);
//    }
//
//    SECTION("auth login fail")
//    {
//        auto key = auth::login("user", "nopass");
//        REQUIRE(!static_cast<bool>(key));
//    }
//
//    auto key = auth::login("user", "pass");
//    result = auth::delete_account(*key);
//    REQUIRE(result);
//}

struct context
{
//    context()
//    {
//        auto result = cpr::Post(cpr::Url{hostname+"/api/v1/user"},cpr::Body{"username=user&password=pass"});
////        REQUIRE(result.status_code==201);
//    }
//
//    ~context()
//    {
////        auto result = cpr::Get(cpr::Url{hostname+"/api/v1/login"}, cpr::Header{{"username", "user"}, {"password", "pass"}});
////        REQUIRE(result.status_code==200);
////        auto body = nlohmann::json::parse(result.text);
////        auto key = body.at("key").get<std::string>();
////
////        result = cpr::Delete(cpr::Url{hostname+"/api/v1/user"}, cpr::Header{{"key", key}});
////        REQUIRE(result.status_code==200);
//    }
};

TEST_CASE("testing authentication over the wire", "[!mayfail]")
{
    auto result = cpr::Post(cpr::Url{hostname+"/api/v1/user"},cpr::Body{"username=user&password=pass"});

    SECTION("auth login basic functioning")
    {
        REQUIRE(false);
    }

    SECTION("auth logout basic functioning")
    {
        REQUIRE(false);
    }

    SECTION("auth login fail")
    {
        REQUIRE(false);
    }
}