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
set(PROJECT_NAME cardtagger_be)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

include(conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)

set(SOURCE_FILES
        main.cpp
        controllers/card_controller.cpp
        controllers/card_controller.h
        models/ability.cpp models/ability.h
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE ${LIBMONGOCXX_INCLUDE_DIRS}
        PRIVATE ${LIBBSONCXX_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE ${CONAN_LIBS}
        PRIVATE ${LIBMONGOCXX_LIBRARIES}
        PRIVATE ${LIBBSONCXX_LIBRARIES}
        )

target_compile_definitions(${PROJECT_NAME}
        PRIVATE ${LIBMONGOCXX_DEFINITIONS}
        PRIVATE ${LIBBSONCXX_DEFINITIONS}
        )


# Tests
add_subdirectory(tests)