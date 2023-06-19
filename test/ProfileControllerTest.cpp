#include "ProfileControllerTest.hpp"
#include "TestData.hpp"
#include "app/TestClient.hpp"
#include "app/TestComponent.hpp"

#include "controller/UserController.hpp"
#include "controller/ProfileController.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"

#include "Poco/URI.h"

#include <cstdio>

void ProfileControllerTest::onRun() {
  /* Register test components */
  TestComponent component;

  /* Create client-server test runner */
  oatpp::test::web::ClientServerTestRunner runner;

  /* Add UserController endpoints to the router of the test server */
  runner.addController(UserController::createShared());
  runner.addController(ProfileController::createShared());
  runner.addController(ProfileControllerOptionalAuth::createShared());

  /* Run test */
  runner.run([this, &runner] {
    /* Get client connection provider for Api Client */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);

    /* Get object mapper component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    /* Create http request executor for Api Client */
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);

    /* Create Test API client */
    auto client = TestClient::createShared(requestExecutor, objectMapper);

    // Start Test
    OATPP_LOGD("ProfileControllerTest", "Login");
    auto loginDto = UserAuthJsonDto::createShared();
    loginDto->user = UserAuthDto::createShared();
    loginDto->user->email = TestData::user[0].email;
    loginDto->user->password = TestData::user[0].password;
    auto response = client->login(loginDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto loginResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(loginResponseDto->user != nullptr);
    OATPP_ASSERT(loginResponseDto->user->username != nullptr && loginResponseDto->user->username->compare(TestData::user[0].username) == 0);
    OATPP_ASSERT(loginResponseDto->user->email != nullptr && loginResponseDto->user->email->compare(TestData::user[0].email) == 0);
    OATPP_ASSERT(loginResponseDto->user->token != nullptr && !loginResponseDto->user->token->empty());
    userToken = "Token " + loginResponseDto->user->token;

    OATPP_LOGD("ProfileControllerTest", "Follow a user");
    response = client->followProfile(TestData::user[1].username, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto followProfileResponseDto = response->readBodyToDto<oatpp::Object<UserProfileJsonDto>>(objectMapper.get());
    OATPP_ASSERT(followProfileResponseDto->profile != nullptr);
    OATPP_ASSERT(followProfileResponseDto->profile->username != nullptr && followProfileResponseDto->profile->username->compare(TestData::user[1].username) == 0);
    OATPP_ASSERT(followProfileResponseDto->profile->following == true);

    OATPP_LOGD("ProfileControllerTest", "Validate the user is being followed");
    response = client->getProfile(TestData::user[1].username, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    followProfileResponseDto = response->readBodyToDto<oatpp::Object<UserProfileJsonDto>>(objectMapper.get());
    OATPP_ASSERT(followProfileResponseDto->profile != nullptr);
    OATPP_ASSERT(followProfileResponseDto->profile->username != nullptr && followProfileResponseDto->profile->username->compare(TestData::user[1].username) == 0);
    OATPP_ASSERT(followProfileResponseDto->profile->following == true);

    OATPP_LOGD("ProfileControllerTest", "Unfollow a user");
    response = client->unfollowProfile(TestData::user[1].username, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);

    OATPP_LOGD("ProfileControllerTest", "Validate the user is no longer being followed");
    response = client->getProfile(TestData::user[1].username, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    followProfileResponseDto = response->readBodyToDto<oatpp::Object<UserProfileJsonDto>>(objectMapper.get());
    OATPP_ASSERT(followProfileResponseDto->profile != nullptr);
    OATPP_ASSERT(followProfileResponseDto->profile->username != nullptr && followProfileResponseDto->profile->username->compare(TestData::user[1].username) == 0);
    OATPP_ASSERT(followProfileResponseDto->profile->following == false);

    OATPP_LOGD("ProfileControllerTest", "Get a non exist user");
    response = client->getProfile(TestData::user[3].username, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 404);
  }, std::chrono::minutes(2) /* test timeout */);

  /* wait all server threads finished */
  std::this_thread::sleep_for(std::chrono::seconds(1));
}