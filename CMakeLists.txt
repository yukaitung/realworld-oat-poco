# CMakeLists.txt

cmake_minimum_required(VERSION 3.23)

project(Realworld
        VERSION 1.0 
        DESCRIPTION "Description"
        HOMEPAGE_URL "https://gitlab.com"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH "bin")

# Prevent building in the source directory

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed.\n")
endif()

# Build 

find_package(oatpp REQUIRED)
find_package(poco REQUIRED Foundation Data DataMySQL)

add_executable(${PROJECT_NAME}
  src/main.cpp

  src/service/UserService.cpp

  src/model/UserModel.cpp
)

target_link_libraries(${PROJECT_NAME}
  oatpp::oatpp
  #oatpp::oatpp-test
  Poco::Foundation
  #Poco::JSON
  #Poco::Net
  #Poco::Redis
  #Poco::XML
  #Poco::Crypto
  Poco::Data
  Poco::DataMySQL
  #Poco::DataPostgreSQL
  #Poco::DataSQLite
  #Poco::Encodings
  Poco::JWT
  #Poco::MongoDB
  #Poco::Util
  #Poco::Zip
  #Poco::ActiveRecord
  #Poco::NetSSL
)

target_include_directories(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/include
  PUBLIC $ENV{HOME}/Develop/mysql/include # Custom MySQL installation
)