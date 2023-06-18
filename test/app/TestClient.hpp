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
  //API_CALL("GET", "/users/{userId}", getUser, PATH(Int32, userId))
  //API_CALL("DELETE", "/users/{userId}", deleteUser, PATH(Int32, userId))
};

#include OATPP_CODEGEN_END(ApiClient)

#endif // TESTCLIENT_HPP