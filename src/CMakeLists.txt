set(SOURCES main.cpp ConverterJSON.cpp InvertedIndex.cpp SearchServer.cpp ${MY_UTILITY_DIR}/simple-thread-pull.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_executable(searchEngine ${SOURCES})

IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -g")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

target_include_directories(searchEngine PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(searchEngine PRIVATE nlohmann_json::nlohmann_json)