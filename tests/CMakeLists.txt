set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SourceFiles
	${MY_SRC_DIR}/InvertedIndex.cpp
	${MY_SRC_DIR}/SearchServer.cpp
	${MY_SRC_DIR}/ConverterJSON.cpp
    ${MY_UTILITY_DIR}/simple-thread-pull.cpp
)

set(TestsFiles
	InvertedIndexTest.cpp
	SearchServerTest.cpp
)

add_executable(googleTests ${TestsFiles} ${SourceFiles})
target_link_libraries(googleTests nlohmann_json::nlohmann_json GTest::gtest_main)
include(GoogleTest)
target_include_directories(googleTests PUBLIC ${MY_INCLUDE_DIR})
gtest_discover_tests(googleTests)