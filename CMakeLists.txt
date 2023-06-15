# CMakeLists.txt

cmake_minimum_required(VERSION 3.23)

project(Realworld VERSION 1.0 DESCRIPTION "Description" HOMEPAGE_URL "https://gitlab.com" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH "bin")

# Prevent building in the source directory

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed.\n")
endif()

# Source Code 

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/ErrorHandler.cpp

  src/helper/Database.cpp
  src/helper/Jwt.cpp
  src/helper/TokenAuthorization.cpp

  src/service/ArticleService.cpp
  src/service/ProfileService.cpp
  src/service/UserService.cpp

  src/model/ArticleHasFavouriteModel.cpp
  src/model/ArticleModel.cpp
  src/model/CommentModel.cpp
  src/model/TagModel.cpp
  src/model/UserHasFollowerModel.cpp
  src/model/UserModel.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/src
  PUBLIC $ENV{HOME}/Develop/mysql/include # Custom MySQL installation
)

# Required Packages

find_package(oatpp REQUIRED)
find_package(oatpp-swagger REQUIRED)
find_package(poco REQUIRED Foundation Data DataMySQL JWT)

target_link_libraries(${PROJECT_NAME}
  PUBLIC oatpp::oatpp
  #oatpp::oatpp-test
  PUBLIC oatpp::oatpp-swagger
  PUBLIC Poco::Foundation
  #Poco::JSON
  #Poco::Net
  #Poco::Redis
  #Poco::XML
  #Poco::Crypto
  PUBLIC Poco::Data
  PUBLIC Poco::DataMySQL
  #Poco::DataPostgreSQL
  #Poco::DataSQLite
  #Poco::Encodings
  PUBLIC Poco::JWT
  #Poco::MongoDB
  #Poco::Util
  #Poco::Zip
  #Poco::ActiveRecord
  #Poco::NetSSL
)

add_definitions(
  ## define path to swagger-ui static resources folder
  -DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res"
)