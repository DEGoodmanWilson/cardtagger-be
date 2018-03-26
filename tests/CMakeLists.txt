#
#       ____________________________
#       \____  \____  \____  \____  \_
#        /   ___/  _/  /   /__/ __/  /
#       /   /__/   /  /   /  / /_/  /
#      /______/___   /___/__/___   /_____________________________
#                /__/  \___    /__/_ \____  \____  \____  \____  \_
#                       /   ___/  _/  /   /__/   /__/   ___/   /__/
#                      /   /  /   /  /   /__    /__    ___/   /  /
#                     /___/  /___   /___   /___   /___   /___/__/
#                               /__/   /__/   /__/   /__/   /
#
#  cardtagger
#  A web application for classifying collectible playing cards
#
#  Copyright © 2018 D.E. Goodman-Wilson
#

cmake_minimum_required(VERSION 3.5)

enable_testing()

add_executable(${PROJECT_NAME}_tests
        main.cpp
        env.h
        auth.cpp
        )

target_link_libraries(${PROJECT_NAME}_tests ${CONAN_LIBS})

add_test(${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}_tests)