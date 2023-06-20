#include "Config.h"
#include "helper/JwtHelper.hpp"
#include "helper/DatabaseHelper.hpp"
#include "model/TagModel.hpp"

#include "oatpp/core/base/Environment.hpp"
#include "UserControllerTest.hpp"
#include "ProfileControllerTest.hpp"
#include "ArticleControllerTest.hpp"

#include "Poco/Crypto/Crypto.h"
#include "Poco/Exception.h"

#include <iostream>

namespace {
  void runTests() {
    OATPP_RUN_TEST(UserControllerTest);
    OATPP_RUN_TEST(ProfileControllerTest);
    OATPP_RUN_TEST(ArticleControllerTest);
  }
}

std::string getEnvVar(std::string const &key)
{
  char *val = getenv(key.c_str());
  return val == NULL ? std::string("") : std::string(val);
}

int main() {
  try {
    Poco::Crypto::initializeCrypto();
  }
  catch(Poco::Exception& exp) {
    //OATPP_LOGE(REALWORLD_PROJECT_NAME, ":%s(): %s", __func__, exp.displayText().c_str());
  }
  
  // Setup Database
  std::string dbHost = getEnvVar("REALWORLD_TEST_DB_HOST");
  if(dbHost.empty()) {
    dbHost = "127.0.0.1";
  }
  std::string dbPort = getEnvVar("REALWORLD_TEST_DB_PORT");
  if(dbPort.empty()) {
    dbPort = "3306";
  }
  std::string dbName = getEnvVar("REALWORLD_TEST_DB_NAME");
  if(dbName.empty()) {
    std::cout << "The database name is missing, terminating\n";
    exit(1);
  }
  std::string dbUser = getEnvVar("REALWORLD_TEST_DB_USER");
  if(dbUser.empty()) {
    std::cout << "The database user is missing, terminating\n";
    exit(1);
  }
  std::string dbPassword = getEnvVar("REALWORLD_TEST_DB_PASSWORD");
  if(dbPassword.empty()) {
    std::cout << "The database password is missing, terminating\n";
    exit(1);
  }
  std::string connectionName = "MySQL";
  DatabaseHelper::initDatabase(connectionName, dbHost, dbPort, dbName, dbUser, dbPassword);

  std::string truncate = getEnvVar("REALWORLD_TEST_TRUNCATE_DB");
  if(truncate.compare("1") == 0) {
    std::cout << "Truncate the database\n";
    DatabaseHelper::testTruncateDatabase();
  }

  std::string signerSecret = getEnvVar("REALWORLD_TEST_SIGNER_SECRET");
  if(signerSecret.empty()) {
    signerSecret = "REALWORLD-OAT-POCO-123456";
  }
  JwtHelper::setSignerSecret(signerSecret);

  // Init cache
  TagModel::initCache();

  oatpp::base::Environment::init();

  runTests();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(oatpp::base::Environment::getObjectsCount() == 0);

  oatpp::base::Environment::destroy();
  
  Poco::Crypto::uninitializeCrypto();

  return 0;
}