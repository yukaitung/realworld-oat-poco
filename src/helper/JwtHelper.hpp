#ifndef JWTHELPER_HPP
#define JWTHELPER_HPP

#include <string>

class JwtHelper {
  private:
    static std::string signerSecret;

  public:
    JwtHelper() = delete;

    /**
    * This method is used to setup jwt signer secret
    */
    static void setSignerSecret(const std::string &secret);

    /**
    * This method is used to issue a JWT token. The token is expired in 15 minutes
    * @param id The user id.
    * @return std::string This returns JWT token.
    */
    static std::string issueJWT(const std::string &id);

    /**
    * This method is used to validate a JWT token.
    * @param jwt The JWT token.
    * @return std::string This returns user id. Returns empty string if the token is invalid.
    */
    static std::string validateJWT(const std::string &jwt);
};

#endif // JWTHELPER_HPP