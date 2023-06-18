#include "UserControllerTest.hpp"
#include "app/TestClient.hpp"
#include "app/TestComponent.hpp"

#include "controller/UserController.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"

#include <cstdio>

std::string UserControllerTest::USER_TOKEN = "Token ";

void UserControllerTest::onRun() {
  /* Register test components */
  TestComponent component;

  /* Create client-server test runner */
  oatpp::test::web::ClientServerTestRunner runner;

  /* Add UserController endpoints to the router of the test server */
  runner.addController(UserController::createShared());

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
    OATPP_LOGD("UserControllerTest", "Create user");
    auto createUserDto = UserRegJsonDto::createShared();
    createUserDto->user = UserRegDto::createShared();
    createUserDto->user->username = USER_NAME;
    createUserDto->user->email = USER_EMAIL;
    createUserDto->user->password = USER_PASSWORD;
    auto response = client->createUser(createUserDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto addedUserResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(addedUserResponseDto->user != nullptr);
    OATPP_ASSERT(addedUserResponseDto->user->username != nullptr && addedUserResponseDto->user->username->compare(USER_NAME) == 0);
    OATPP_ASSERT(addedUserResponseDto->user->email != nullptr && addedUserResponseDto->user->email->compare(USER_EMAIL) == 0);
    OATPP_ASSERT(addedUserResponseDto->user->token != nullptr && !addedUserResponseDto->user->token->empty());

    OATPP_LOGD("UserControllerTest", "Create user, username taken");
    createUserDto->user->username = USER_NAME;
    createUserDto->user->email = USER_EMAIL + "1";
    response = client->createUser(createUserDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);

    OATPP_LOGD("UserControllerTest", "Create user, email taken");
    createUserDto->user->username = USER_NAME + "1";
    createUserDto->user->email = USER_EMAIL;
    response = client->createUser(createUserDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);

    OATPP_LOGD("UserControllerTest", "Create srcond user");
    createUserDto->user->username = USER_NAME + "1";
    createUserDto->user->email = USER_EMAIL + "1";
    response = client->createUser(createUserDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    
    OATPP_LOGD("UserControllerTest", "Login");
    auto loginDto = UserAuthJsonDto::createShared();
    loginDto->user = UserAuthDto::createShared();
    loginDto->user->email = USER_EMAIL;
    loginDto->user->password = USER_PASSWORD;
    response = client->login(loginDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto loginResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(loginResponseDto->user != nullptr);
    OATPP_ASSERT(loginResponseDto->user->username != nullptr && loginResponseDto->user->username->compare(USER_NAME) == 0);
    OATPP_ASSERT(loginResponseDto->user->email != nullptr && loginResponseDto->user->email->compare(USER_EMAIL) == 0);
    OATPP_ASSERT(loginResponseDto->user->token != nullptr && !loginResponseDto->user->token->empty());
    USER_TOKEN += loginResponseDto->user->token;

    OATPP_LOGD("UserControllerTest", "Login, user not exist");
    loginDto->user->email = USER_EMAIL + "Error";
    response = client->login(loginDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 404);

    OATPP_LOGD("UserControllerTest", "Login, incorrect password");
    loginDto->user->email = USER_EMAIL;
    loginDto->user->password = USER_PASSWORD + "Error";
    response = client->login(loginDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);

    OATPP_LOGD("UserControllerTest", "Get user");
    response = client->getUser(USER_TOKEN);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto getUserResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(getUserResponseDto->user != nullptr);
    OATPP_ASSERT(getUserResponseDto->user->username != nullptr && getUserResponseDto->user->username->compare(USER_NAME) == 0);
    OATPP_ASSERT(getUserResponseDto->user->email != nullptr && getUserResponseDto->user->email->compare(USER_EMAIL) == 0);
    OATPP_ASSERT(getUserResponseDto->user->token != nullptr && !getUserResponseDto->user->token->empty());

    OATPP_LOGD("UserControllerTest", "Update user")
    auto updateUserDto = UserUpdateJsonDto::createShared();
    updateUserDto->user = UserUpdateDto::createShared();
    updateUserDto->user->username = USER_NAME + "0";
    updateUserDto->user->email = USER_EMAIL + "0";
    updateUserDto->user->password = USER_PASSWORD + "0";
    updateUserDto->user->bio = USER_BIO;
    updateUserDto->user->image = USER_IMAGE;
    response = client->updateUser(updateUserDto, USER_TOKEN);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto updateUserResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(updateUserResponseDto->user != nullptr);
    OATPP_ASSERT(updateUserResponseDto->user->username != nullptr && updateUserResponseDto->user->username->compare(USER_NAME + "0") == 0);
    OATPP_ASSERT(updateUserResponseDto->user->email != nullptr && updateUserResponseDto->user->email->compare(USER_EMAIL + "0") == 0);
    OATPP_ASSERT(updateUserResponseDto->user->token != nullptr && !updateUserResponseDto->user->token->empty());
    OATPP_ASSERT(updateUserResponseDto->user->bio != nullptr && updateUserResponseDto->user->bio->compare(USER_BIO) == 0);
    OATPP_ASSERT(updateUserResponseDto->user->image != nullptr && updateUserResponseDto->user->image->compare(USER_IMAGE) == 0);
    USER_TOKEN = "Token " + updateUserResponseDto->user->token; 

    OATPP_LOGD("UserControllerTest", "Update user, username taken")
    updateUserDto->user->username = USER_NAME + "1";
    updateUserDto->user->email = USER_EMAIL + "0";
    response = client->updateUser(updateUserDto, USER_TOKEN);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);

    OATPP_LOGD("UserControllerTest", "Update user, email taken")
    updateUserDto->user->username = USER_NAME + "0";
    updateUserDto->user->email = USER_EMAIL + "1";
    response = client->updateUser(updateUserDto, USER_TOKEN);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);
  }, std::chrono::minutes(10) /* test timeout */);

  /* wait all server threads finished */
  std::this_thread::sleep_for(std::chrono::seconds(1));
}