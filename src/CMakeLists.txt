set(SOURCES main.cpp ConverterJSON.cpp InvertedIndex.cpp SearchServer.cpp ${MY_UTILITY_DIR}/simple-thread-pull.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_executable(searchEngine ${SOURCES})

target_include_directories(searchEngine PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(searchEngine PRIVATE nlohmann_json::nlohmann_json)