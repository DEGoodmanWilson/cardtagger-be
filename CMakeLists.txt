cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME cardtagger_be)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

include(conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)

set(SOURCE_FILES
        magique/card.h magique/card.cpp
        magique/catalog.h magique/catalog.cpp
        main.cpp
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