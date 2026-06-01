cmake_minimum_required(VERSION 3.20)
project(sfml_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)


set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-2.6.2/lib/cmake/SFML")

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

add_executable(sfml_project main.cpp
        Game.cpp
        Game.h
        Player.cpp
        Player.h
        Map.cpp
        Map.h
        Mob.cpp
        Mob.h
        FastMob.cpp
        FastMob.h
        TankMob.cpp
        TankMob.h)


target_include_directories(sfml_project PRIVATE
        "${CMAKE_SOURCE_DIR}/external/SFML-2.6.2/include"
)

target_link_libraries(sfml_project PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)