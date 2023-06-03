#include "helper/Database.hpp"

std::shared_ptr<Poco::Data::SessionPool> Database::pool = nullptr;

void Database::setUpDatabase(std::string connectionName, std::string connectionString) {
  if(Database::pool != nullptr) {
    return;
  }
  Poco::Data::MySQL::Connector::registerConnector();
  Database::pool = std::make_shared<Poco::Data::SessionPool>(connectionName, connectionString);
};