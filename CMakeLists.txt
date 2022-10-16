cmake_minimum_required(VERSION 3.0.0)
project(searchEngine VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

set(VERSION_OF_CONFIG "0.1.0")
set(NAME_OF_CONFIG_FILE "config.json")
set(NAME_OF_ANSWERS_FILE "answers.json")
set(NAME_OF_REQUESTS_FILE "requests.json")

configure_file (config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.hpp @ONLY)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

set(MY_UTILITY_DIR ${MY_SRC_DIR}/utility)

set(MY_INCLUDE_DIR 
  ${CMAKE_SOURCE_DIR}/include 
  ${MY_UTILITY_DIR}
)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)