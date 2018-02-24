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


#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

std::string hostname{"http://localhost"};

int main( int argc, char* argv[] )
{
    if (auto port = std::getenv("PORT"))
    {
        hostname = hostname + ":" + port;
    }
    else
    {
        hostname = hostname + ":8080";
    }

    int result = Catch::Session().run( argc, argv );

    return ( result < 0xff ? result : 0xff );
}