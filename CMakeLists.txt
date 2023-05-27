# CMakeLists.txt

cmake_minimum_required(VERSION 3.23)

project(Realworld
        VERSION 1.0 
        DESCRIPTION "Description"
        HOMEPAGE_URL "https://gitlab.com"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Prevent building in the source directory

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed.\n")
endif()

# Build 

find_package(oatpp REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
  oatpp::oatpp
  #oatpp::oatpp-test
)

target_include_directories(${PROJECT_NAME}
  PRIVATE 
  ${PROJECT_SOURCE_DIR}/include
)