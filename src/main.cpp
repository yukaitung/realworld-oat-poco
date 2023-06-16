#include "Application.hpp"
#include "helper/Database.hpp"
#include "Config.h"

#include "controller/ArticleController.hpp"
#include "controller/ProfileController.hpp"
#include "controller/UserController.hpp"

#include "model/TagModel.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"

#include <iostream>
#include <string>

std::string getEnvVar(std::string const &key)
{
  char *val = getenv(key.c_str());
  return val == NULL ? std::string("") : std::string(val);
}

void run() {
  Application app;

  // Setup Database
  std::string dbHost = getEnvVar("REALWORLD_DB_HOST");
  std::string dbPort = getEnvVar("REALWORLD_DB_PORT");
  std::string dbName = getEnvVar("REALWORLD_DB_NAME");
  std::string dbUser = getEnvVar("REALWORLD_DB_USER");
  std::string dbPassword = getEnvVar("REALWORLD_DB_PASSWORD");
  std::string connectionName = "MySQL";
  std::string connectionString = "host=" + dbHost + ";port=" + dbPort + ";db=" + dbName + ";user=" + dbUser + ";password=" + dbPassword + ";compress=true;auto-reconnect=true";
  Database::InitDatabase(connectionName, connectionString);

  // Init cache
  TagModel::InitCache();

  auto router = app.httpRouter.getObject();
  oatpp::web::server::api::Endpoints docEndpoints;

  docEndpoints.append(router->addController(UserController::createShared())->getEndpoints());
  docEndpoints.append(router->addController(ProfileController::createShared())->getEndpoints());
  docEndpoints.append(router->addController(ProfileControllerOptionalAuth::createShared())->getEndpoints());
  docEndpoints.append(router->addController(ArticleController::createShared())->getEndpoints());
  docEndpoints.append(router->addController(ArticleControllerOptionalAuth::createShared())->getEndpoints());
  router->addController(oatpp::swagger::Controller::createShared(docEndpoints));

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* create server */
  oatpp::network::Server server(connectionProvider, connectionHandler);
  
  OATPP_LOGD(REALWORLD_PROJECT_NAME, "%s Running on port %s...", REALWORLD_PROJECT_NAME, connectionProvider->getProperty("port").toString()->c_str());
  
  server.run();
}

int main (int argc, const char * argv[])
{
  oatpp::base::Environment::init();
  
  run();
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
  
  return 0;
}