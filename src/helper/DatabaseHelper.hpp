#ifndef DATABASEHELPER_HPP
#define DATABASEHELPER_HPP

#include "Poco/Data/SessionPool.h"
#include "Poco/Data/MySQL/Connector.h"

#include <string>
#include <memory>

class DatabaseHelper {
  private:
    static std::shared_ptr<Poco::Data::SessionPool> pool;

  public:
    DatabaseHelper() = delete;

    /**
    * This method is to establish a connection to database
    * @param connectionName The connection name.
    * @param dbHost The database ip address.
    * @param dbPort The database port.
    * @param dbName The database name.
    * @param dbUser The database username.
    * @param dbPassword The database userpassword.
    */
    static void initDatabase(const std::string &connectionName, const std::string &dbHost, const std::string &dbPort, const std::string &dbName, const std::string &dbUser, const std::string &dbPassword);

    /**
    * This method is to obtain database session from pool
    * @return Poco::Data::Session This returns a database session.
    */
    static Poco::Data::Session getSession() {return DatabaseHelper::pool->get();};

    /**
    * This method is to obtain truncate the database. Please use it in unit test only.
    */
    static void testTruncateDatabase();
};

#endif // DATABASEHELPER_HPP