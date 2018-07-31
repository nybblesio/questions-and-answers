cmake_minimum_required(VERSION 3.12)
project(sdl2_timers)
set(CMAKE_CXX_STANDARD 17)

set(SDL_SHARED OFF)
add_subdirectory(ext/SDL2-2.0.8)
include_directories(ext/SDL2-2.0.8/include)

add_executable(
        sdl2_timers
        main.cpp
)
target_link_libraries(
        sdl2_timers
        SDL2-static
)
