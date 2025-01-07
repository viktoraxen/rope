find_package(GTest REQUIRED)

# TODO: Add source files needed for tests
add_executable(RopeTest main.cpp tests.cpp ${CMAKE_SOURCE_DIR}/src/rope.cpp)

target_link_libraries(RopeTest GTest::gtest GTest::gtest_main pthread)

target_include_directories(RopeTest PRIVATE ${CMAKE_SOURCE_DIR}/include)

gtest_discover_tests(RopeTest PROPERTIES ENVIRONMENT "GTEST_COLOR=1")
