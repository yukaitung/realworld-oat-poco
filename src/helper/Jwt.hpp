#ifndef JWT_HPP
#define JWT_HPP

#include <string>

class Jwt {
  public:
    Jwt() = delete;
    static std::string issueJWT(const std::string &id);
    static std::string validateJWT(const std::string &jwt);
};

#endif // JWT_HPP