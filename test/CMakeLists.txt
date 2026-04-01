cmake_minimum_required(VERSION 3.20)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_tests
  src/test_main.cpp
  src/test_store.cpp
  src/test_nats_client.cpp
  src/test_routes.cpp
)

target_compile_features(${PROJECT_NAME}_tests PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME}_tests PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(${PROJECT_NAME}_tests
  PRIVATE
    ${PROJECT_NAME}::${PROJECT_NAME}
    ${PROJECT_NAME}::core
    GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
