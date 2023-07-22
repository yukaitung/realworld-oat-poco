
#ifndef USERCONTROLLER_HPP
#define USERCONTROLLER_HPP

#include "dto/StatusDto.hpp"

#include "service/UserService.hpp"
#include "helper/TokenAuthorization.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * User REST controller.
 */
class UserController : public oatpp::web::server::api::ApiController {
private:
  UserService userService;

public:
  UserController(const std::shared_ptr<ObjectMapper>& objectMapper) : oatpp::web::server::api::ApiController(objectMapper) {
    setDefaultAuthorizationHandler(std::make_shared<TokenAuthorizationHandler>());
  }

  static std::shared_ptr<UserController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<UserController>(objectMapper);
  }

  ADD_CORS(createUser)
  ENDPOINT_INFO(createUser) {
    info->summary = "Create new user";
    info->addConsumes<Object<UserRegJsonDto>>("application/json");
    info->addResponse<Object<UserJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addTag("UserController");
  }
  ENDPOINT("POST", "/users", createUser, BODY_DTO(Object<UserRegJsonDto>, dto))
  {
    return createDtoResponse(Status::CODE_200, userService.createUser(dto));
  }

  ADD_CORS(login)
  ENDPOINT_INFO(login) {
    info->summary = "Login";
    info->addResponse<Object<UserJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addTag("UserController");
  }
  ENDPOINT("POST", "/users/login", login, BODY_DTO(Object<UserAuthJsonDto>, dto))
  {
    return createDtoResponse(Status::CODE_200, userService.login(dto));
  }

  ADD_CORS(getUser)
  ENDPOINT_INFO(getUser) {
    info->summary = "Get current user";
    info->addResponse<Object<UserJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("UserController");
  }
  ENDPOINT("GET", "/user", getUser, AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    return createDtoResponse(Status::CODE_200, userService.getUser(authObject->id));
  }

  ADD_CORS(updateUser)
  ENDPOINT_INFO(updateUser) {
    info->summary = "Update information for current user";
    info->addResponse<Object<UserJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("UserController");
  }
  ENDPOINT("PUT", "/user", updateUser, BODY_DTO(Object<UserUpdateJsonDto>, dto), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    return createDtoResponse(Status::CODE_200, userService.updateUser(authObject->id, dto));
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif // USERCONTROLLER_HPP
