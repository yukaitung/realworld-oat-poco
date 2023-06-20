# CMakeLists.txt

cmake_minimum_required(VERSION 3.23)

project(Realworld VERSION 1.0 DESCRIPTION "C++, POCO, Oat++ codebase containing real-world examples (CRUD, auth, MVC patterns, etc.)" HOMEPAGE_URL "https://gitlab.com/yukaitung" LANGUAGES CXX)
set(CONTACT_NAME "Yu Kai Tung")
set(PROJECT_FULL_TITLE "RealWorld Example App")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH "bin")

set_target_properties(${TARGET_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED OFF
  CXX_EXTENSIONS OFF
)

# Prevent building in the source directory

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed.\n")
endif()

# Project source code

configure_file(src/Config.h.in ${PROJECT_SOURCE_DIR}/src/Config.h)
add_library(${PROJECT_NAME}-lib
  src/helper/DatabaseHelper.cpp
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
target_include_directories(${PROJECT_NAME}-lib
  PUBLIC ${PROJECT_SOURCE_DIR}/src
)

# Required packages

find_package(oatpp REQUIRED)
find_package(oatpp-swagger REQUIRED)
find_package(Poco REQUIRED Foundation Data DataMySQL JWT)

target_link_libraries(${PROJECT_NAME}-lib
  PUBLIC oatpp::oatpp
  PUBLIC oatpp::oatpp-test
  PUBLIC oatpp::oatpp-swagger
  PUBLIC Poco::Foundation
  #Poco::JSON
  #Poco::Net
  #Poco::Redis
  #Poco::XML
  PUBLIC Poco::Crypto
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

# Output executable

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/ErrorHandler.cpp
)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)

# Copy swagger res folder
file(COPY "${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_OUTPUT_PATH})

# Output test

add_executable(${PROJECT_NAME}-test
  test/tests.cpp
  test/TestData.cpp

  test/app/TestClient.hpp
  test/app/TestComponent.hpp

  test/ArticleControllerTest.hpp
  test/ArticleControllerTest.cpp
  test/UserControllerTest.hpp
  test/UserControllerTest.cpp
  test/ProfileControllerTest.hpp
  test/ProfileControllerTest.cpp
)
target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME}-lib)
enable_testing()
add_test(project-tests ${PROJECT_NAME}-test)