#ifndef TESTDATA_HPP
#define TESTDATA_HPP

#include <string>
#include <vector>

class User {
  public:
    std::string username;
    std::string email;
    std::string password;
    std::string bio;
    std::string image;
};

class TestData {
  public:
    const static std::vector<User> user;
};

#endif // TESTDATA_HPP