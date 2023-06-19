#include "UserControllerTest.hpp"
#include "TestData.hpp"
#include "app/TestClient.hpp"
#include "app/TestComponent.hpp"

#include "controller/UserController.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"

#include <cstdio>

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
    createUserDto->user->username = TestData::user[0].username;
    createUserDto->user->email = TestData::user[0].email;
    createUserDto->user->password = TestData::user[0].password;
    auto response = client->createUser(createUserDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto addedUserResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(addedUserResponseDto->user != nullptr);
    OATPP_ASSERT(addedUserResponseDto->user->username != nullptr && addedUserResponseDto->user->username->compare(TestData::user[0].username) == 0);
    OATPP_ASSERT(addedUserResponseDto->user->email != nullptr && addedUserResponseDto->user->email->compare(TestData::user[0].email) == 0);
    OATPP_ASSERT(addedUserResponseDto->user->token != nullptr && !addedUserResponseDto->user->token->empty());

    OATPP_LOGD("UserControllerTest", "Create user, username taken");
    createUserDto->user->username = TestData::user[0].username;
    createUserDto->user->email = TestData::user[1].email;
    response = client->createUser(createUserDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);

    OATPP_LOGD("UserControllerTest", "Create user, email taken");
    createUserDto->user->username = TestData::user[1].username;
    createUserDto->user->email = TestData::user[0].email;
    response = client->createUser(createUserDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);

    OATPP_LOGD("UserControllerTest", "Create second user");
    createUserDto->user->username = TestData::user[1].username;
    createUserDto->user->email = TestData::user[1].email;
    response = client->createUser(createUserDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    
    OATPP_LOGD("UserControllerTest", "Login");
    auto loginDto = UserAuthJsonDto::createShared();
    loginDto->user = UserAuthDto::createShared();
    loginDto->user->email = TestData::user[0].email;
    loginDto->user->password = TestData::user[0].password;
    response = client->login(loginDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto loginResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(loginResponseDto->user != nullptr);
    OATPP_ASSERT(loginResponseDto->user->username != nullptr && loginResponseDto->user->username->compare(TestData::user[0].username) == 0);
    OATPP_ASSERT(loginResponseDto->user->email != nullptr && loginResponseDto->user->email->compare(TestData::user[0].email) == 0);
    OATPP_ASSERT(loginResponseDto->user->token != nullptr && !loginResponseDto->user->token->empty());
    userToken = "Token " + loginResponseDto->user->token;

    OATPP_LOGD("UserControllerTest", "Login, user not exist");
    loginDto->user->email = TestData::user[2].email;
    response = client->login(loginDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 404);

    OATPP_LOGD("UserControllerTest", "Login, incorrect password");
    loginDto->user->email = TestData::user[0].email;
    loginDto->user->password = TestData::user[1].password;
    response = client->login(loginDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);

    OATPP_LOGD("UserControllerTest", "Get user");
    response = client->getUser(userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto getUserResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(getUserResponseDto->user != nullptr);
    OATPP_ASSERT(getUserResponseDto->user->username != nullptr && getUserResponseDto->user->username->compare(TestData::user[0].username) == 0);
    OATPP_ASSERT(getUserResponseDto->user->email != nullptr && getUserResponseDto->user->email->compare(TestData::user[0].email) == 0);
    OATPP_ASSERT(getUserResponseDto->user->token != nullptr && !getUserResponseDto->user->token->empty());

    OATPP_LOGD("UserControllerTest", "Update user")
    auto updateUserDto = UserUpdateJsonDto::createShared();
    updateUserDto->user = UserUpdateDto::createShared();
    updateUserDto->user->username = TestData::user[2].username;
    updateUserDto->user->email = TestData::user[2].email;
    updateUserDto->user->password = TestData::user[2].password;
    updateUserDto->user->bio = TestData::user[2].bio;
    updateUserDto->user->image = TestData::user[2].image;
    response = client->updateUser(updateUserDto, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto updateUserResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(updateUserResponseDto->user != nullptr);
    OATPP_ASSERT(updateUserResponseDto->user->username != nullptr && updateUserResponseDto->user->username->compare(TestData::user[2].username) == 0);
    OATPP_ASSERT(updateUserResponseDto->user->email != nullptr && updateUserResponseDto->user->email->compare(TestData::user[2].email) == 0);
    OATPP_ASSERT(updateUserResponseDto->user->token != nullptr && !updateUserResponseDto->user->token->empty());
    OATPP_ASSERT(updateUserResponseDto->user->bio != nullptr && updateUserResponseDto->user->bio->compare(TestData::user[2].bio) == 0);
    OATPP_ASSERT(updateUserResponseDto->user->image != nullptr && updateUserResponseDto->user->image->compare(TestData::user[2].image) == 0);
    userToken = "Token " + updateUserResponseDto->user->token;

    OATPP_LOGD("UserControllerTest", "Validate user is updated");
    response = client->getUser(userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    getUserResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(getUserResponseDto->user != nullptr);
    OATPP_ASSERT(getUserResponseDto->user->username != nullptr && getUserResponseDto->user->username->compare(TestData::user[2].username) == 0);
    OATPP_ASSERT(getUserResponseDto->user->email != nullptr && getUserResponseDto->user->email->compare(TestData::user[2].email) == 0);
    OATPP_ASSERT(getUserResponseDto->user->token != nullptr && !getUserResponseDto->user->token->empty());
    OATPP_ASSERT(getUserResponseDto->user->bio != nullptr && getUserResponseDto->user->bio->compare(TestData::user[2].bio) == 0);
    OATPP_ASSERT(getUserResponseDto->user->image != nullptr && getUserResponseDto->user->image->compare(TestData::user[2].image) == 0);

    OATPP_LOGD("UserControllerTest", "Update user, username taken")
    updateUserDto->user->username = TestData::user[1].username;
    updateUserDto->user->email = TestData::user[0].email;
    response = client->updateUser(updateUserDto, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);

    OATPP_LOGD("UserControllerTest", "Update user, email taken")
    updateUserDto->user->username = TestData::user[0].username;
    updateUserDto->user->email = TestData::user[1].email;
    response = client->updateUser(updateUserDto, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 422);

    OATPP_LOGD("UserControllerTest", "Create third user");
    createUserDto->user = UserRegDto::createShared();
    createUserDto->user->username = TestData::user[0].username;
    createUserDto->user->email = TestData::user[0].email;
    createUserDto->user->password = TestData::user[0].password;
    response = client->createUser(createUserDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
  }, std::chrono::minutes(2) /* test timeout */);

  /* wait all server threads finished */
  std::this_thread::sleep_for(std::chrono::seconds(1));
}