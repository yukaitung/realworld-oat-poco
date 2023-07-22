
#ifndef PROFILECONTROLLER_HPP
#define PROFILECONTROLLER_HPP

#include "controller/ControllerBase.hpp"
#include "service/ProfileService.hpp"
#include "helper/TokenAuthorization.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "Poco/URI.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * Profile REST controller.
 */
class ProfileController : public ControllerBase {
private:
  ProfileService profileService;

public:
  ProfileController(const std::shared_ptr<ObjectMapper>& objectMapper) : ControllerBase(objectMapper) {
    setDefaultAuthorizationHandler(std::make_shared<TokenAuthorizationHandler>());
  }

  ENDPOINT_INFO(followProfile) {
    info->summary = "Follow a user";
    info->addResponse<Object<UserProfileJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ProfileController");
  }
  ENDPOINT("POST", "/profiles/{username}/follow", followProfile, PATH(String, username), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    // Fix MySql utf8mb3_general_ci complain
    std::string usernameStr1 = username.getValue("");
    std::string usernameStr2 = "";
    Poco::URI::decode(usernameStr1, usernameStr2);
    return createCORSDtoResponse(Status::CODE_200, profileService.followProfile(authObject->id, usernameStr2));
  }

  ENDPOINT_INFO(unfollowProfile) {
    info->summary = "Unfollow a user";
    info->addResponse<Object<UserProfileJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ProfileController");
  }
  ENDPOINT("DELETE", "/profiles/{username}/follow", unfollowProfile, PATH(String, username), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    // Fix MySql utf8mb3_general_ci complain
    std::string usernameStr1 = username.getValue("");
    std::string usernameStr2 = "";
    Poco::URI::decode(usernameStr1, usernameStr2);
    return createCORSDtoResponse(Status::CODE_200, profileService.unfollowProfile(authObject->id, usernameStr2));
  }

  static std::shared_ptr<ProfileController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<ProfileController>(objectMapper);
  }
};

/**
 * Profile REST controller, the authentication is optional.
 */
class ProfileControllerOptionalAuth : public ControllerBase {
private:
  ProfileService profileService;

public:
  ProfileControllerOptionalAuth(const std::shared_ptr<ObjectMapper>& objectMapper) : ControllerBase(objectMapper) {
    setDefaultAuthorizationHandler(std::make_shared<OptionalTokenAuthorizationHandler>());
  }

  static std::shared_ptr<ProfileControllerOptionalAuth> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<ProfileControllerOptionalAuth>(objectMapper);
  }

  ENDPOINT_INFO(getProfile) {
    info->summary = "Get a user profile (Authorization is optional)";
    info->addResponse<Object<UserProfileJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ProfileController");
  }
  ENDPOINT("GET", "/profiles/{username}", getProfile, PATH(String, username), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    // Fix MySql utf8mb3_general_ci complain
    std::string usernameStr1 = username.getValue("");
    std::string usernameStr2 = "";
    Poco::URI::decode(usernameStr1, usernameStr2);
    return createCORSDtoResponse(Status::CODE_200, profileService.getProfile(authObject->id, usernameStr2));
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif // PROFILECONTROLLER_HPP
