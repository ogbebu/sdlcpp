cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED true)
project(sdlcpp)

add_executable(sdlcpp src/main.cpp)
# Tell SDL to build static, not shared
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)   # <-- add this

add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)
add_subdirectory(third_party/imgui)
add_subdirectory(third_party/SDL_image EXCLUDE_FROM_ALL)

target_link_libraries(sdlcpp PRIVATE "SDL3::SDL3-static" imgui SDL3_image::SDL3_image)