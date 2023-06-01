#ifndef JWT_HPP
#define JWT_HPP

#include <string>

class Jwt {
  public:
    Jwt() {};
    static std::string issueJWT(const std::string &username);
    static std::string validateJWT(const std::string &jwt);
};

#endif // JWT_HPP