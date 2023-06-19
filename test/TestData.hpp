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

class Article {
  public:
    std::string title;
    std::string description;
    std::string body;
    std::vector<int> tagId;
};

class TestData {
  public:
    const static std::vector<User> user;
    const static std::vector<Article> article;
    const static std::vector<std::string> tag;
    const static std::vector<std::string> comment;
};

#endif // TESTDATA_HPP