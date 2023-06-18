#ifndef TESTCLIENT_HPP
#define TESTCLIENT_HPP

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "dto/UserDto.hpp"

#include OATPP_CODEGEN_BEGIN(ApiClient)

/**
 * Test API client.
 * Use this client to call application APIs.
 */
class TestClient : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(TestClient)

  // User Controller
  API_CALL("POST", "/users", createUser, BODY_DTO(Object<UserRegJsonDto>, dto))
  API_CALL("POST", "/users/login", login, BODY_DTO(Object<UserAuthJsonDto>, dto))
  API_CALL("GET", "/user", getUser, HEADER(String, token, "Authorization"))
  API_CALL("PUT", "/user", updateUser, BODY_DTO(Object<UserUpdateJsonDto>, dto), HEADER(String, token, "Authorization"))
};

#include OATPP_CODEGEN_END(ApiClient)

#endif // TESTCLIENT_HPP