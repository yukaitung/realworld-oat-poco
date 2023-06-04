#ifndef TOKENAUTHORIZATION_HPP
#define TOKENAUTHORIZATION_HPP

#include "oatpp/web/server/handler/AuthorizationHandler.hpp"
#include "model/UserModel.hpp"

class TokenAuthorizationObject : public oatpp::web::server::handler::AuthorizationObject {
  public:
    std::string id;
    TokenAuthorizationObject(const std::string &pId) : id(pId) {}
};

class TokenAuthorizationHandler : public oatpp::web::server::handler::BearerAuthorizationHandler {
  private:
    UserModel userModel;

  public:
    TokenAuthorizationHandler() : BearerAuthorizationHandler("realworld") {}
    std::shared_ptr<AuthorizationObject> handleAuthorization(const oatpp::String &header) override;
    std::shared_ptr<AuthorizationObject> authorize(const oatpp::String& token) override;
};

class OptionalTokenAuthorizationHandler : public oatpp::web::server::handler::BearerAuthorizationHandler {
  private:
    UserModel userModel;

  public:
    OptionalTokenAuthorizationHandler() : BearerAuthorizationHandler("realworld") {}
    std::shared_ptr<AuthorizationObject> handleAuthorization(const oatpp::String &header) override;
    std::shared_ptr<AuthorizationObject> authorize(const oatpp::String& token) override;
};

#endif // TOKENAUTHORIZATIONOBJECT_HPP