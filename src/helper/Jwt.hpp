#ifndef JWT_HPP
#define JWT_HPP

#include <string>

class Jwt {
  public:
    Jwt() = delete;

    /**
    * This method is used to issue a JWT token.
    * @param id The user id.
    * @return std::string This returns JWT token.
    */
    static std::string issueJWT(const std::string &id);

    /**
    * This method is used to validate a JWT token.
    * @param jwt The JWT token.
    * @return std::string This returns user id.
    */
    static std::string validateJWT(const std::string &jwt);
};

#endif // JWT_HPP