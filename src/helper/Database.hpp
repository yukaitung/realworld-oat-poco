#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "Poco/Data/SessionPool.h"
#include "Poco/Data/MySQL/Connector.h"

#include <string>
#include <memory>

class Database {
  private:
    static std::shared_ptr<Poco::Data::SessionPool> pool;
  public:
    Database() = delete;
    static void setUpDatabase(std::string connectionName, std::string connectionString);
    static std::shared_ptr<Poco::Data::SessionPool> getPool() {return Database::pool;};
};

#endif // DATABASE_HPP