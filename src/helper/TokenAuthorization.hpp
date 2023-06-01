#ifndef TOKENAUTHORIZATION_HPP
#define TOKENAUTHORIZATION_HPP

#include "oatpp/web/server/handler/AuthorizationHandler.hpp"
#include "model/UserModel.hpp"

class TokenAuthorizationObject : public oatpp::web::server::handler::AuthorizationObject {
  public:
    std::string username;
    TokenAuthorizationObject(const std::string &pUsername) : username(pUsername) {}
};

class TokenAuthorizationHandler : public oatpp::web::server::handler::BearerAuthorizationHandler {
  private:
    UserModel userModel;

  public:
    TokenAuthorizationHandler() : BearerAuthorizationHandler("realworld") {}
    std::shared_ptr<AuthorizationObject> handleAuthorization(const oatpp::String &header) override;
    std::shared_ptr<AuthorizationObject> authorize(const oatpp::String& token) override;
};

#endif // TOKENAUTHORIZATIONOBJECT_HPP