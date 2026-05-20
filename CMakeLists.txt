cmake_minimum_required(VERSION 3.10)
project(DungeonCrawler CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DungeonCrawler
    main.cpp
    src/Juego.cpp
    src/Mapa.cpp
    src/Jugador.cpp
    src/Enemigo.cpp
    src/Objeto.cpp
    src/Consola.cpp
)

target_include_directories(DungeonCrawler PRIVATE include)