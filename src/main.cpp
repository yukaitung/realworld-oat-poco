#include "Application.hpp"
#include "helper/Database.hpp"

#include "controller/ArticleController.hpp"
#include "controller/ProfileController.hpp"
#include "controller/UserController.hpp"

#include "model/TagModel.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"

#include <iostream>

void run() {
  Application app;

  // Setup Database
  // TODO: dont insert secret
  std::string connectionName = "MySQL";
  std::string connectionString = "host=127.0.0.1;port=3306;db=Realworld;user=root;password=GueBPjHlPFUgXc7hm=;compress=true;auto-reconnect=true";
  Database::InitDatabase(connectionName, connectionString);

  // Init cache
  TagModel::InitCache();

  auto router = app.httpRouter.getObject();
  oatpp::web::server::api::Endpoints docEndpoints;

  docEndpoints.append(router->addController(UserController::createShared())->getEndpoints());
  router->addController(oatpp::swagger::Controller::createShared(docEndpoints)); // One by one
  docEndpoints.append(router->addController(ArticleController::createShared())->getEndpoints());
  docEndpoints.append(router->addController(ArticleControllerOptionalAuth::createShared())->getEndpoints());
  docEndpoints.append(router->addController(ProfileController::createShared())->getEndpoints());
  docEndpoints.append(router->addController(ProfileControllerOptionalAuth::createShared())->getEndpoints());

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* create server */
  oatpp::network::Server server(connectionProvider, connectionHandler);
  
  OATPP_LOGD("Server", "Running on port %s...", connectionProvider->getProperty("port").toString()->c_str());
  
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