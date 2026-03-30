cmake_minimum_required(VERSION 3.20)

project(
  ProjectNestor
  VERSION 0.1.0
  DESCRIPTION "Research, ideation, and search for HomericIntelligence"
  LANGUAGES CXX)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory and run CMake from there."
  )
endif()

include(cmake/StandardSettings.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Doxygen.cmake)
include(cmake/SourcesAndHeaders.cmake)

add_library(${PROJECT_NAME} ${headers} ${sources})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE ${PROJECT_SOURCE_DIR}/src)

set_project_warnings(${PROJECT_NAME})

# ─── HTTP Server Dependencies ───────────────────────────────────────────────
include(FetchContent)

FetchContent_Declare(
  cpp-httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG        v0.18.3
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(
  natsc
  GIT_REPOSITORY https://github.com/nats-io/nats.c.git
  GIT_TAG        v3.9.1
  GIT_SHALLOW    TRUE
)

set(NATS_BUILD_STREAMING OFF CACHE BOOL "" FORCE)
set(NATS_BUILD_EXAMPLES  OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING        OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(cpp-httplib nlohmann_json natsc)

# ─── Server Executable ─────────────────────────────────────────────────────
add_executable(ProjectNestor_server
  src/server_main.cpp
  src/routes.cpp
  src/store.cpp
  src/nats_client.cpp
)

target_include_directories(ProjectNestor_server PRIVATE
  include/
  ${cpp-httplib_SOURCE_DIR}
  ${nlohmann_json_SOURCE_DIR}/include
  ${natsc_SOURCE_DIR}/include
)

target_link_libraries(ProjectNestor_server PRIVATE
  httplib::httplib
  nlohmann_json::nlohmann_json
  nats_static
)

target_compile_features(ProjectNestor_server PRIVATE cxx_std_20)

if(${PROJECT_NAME}_BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

if(${PROJECT_NAME}_ENABLE_DOXYGEN)
  enable_doxygen()
endif()

include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
