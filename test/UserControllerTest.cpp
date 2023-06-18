#include "UserControllerTest.hpp"
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
    auto dto = UserRegJsonDto::createShared();
    dto->user = UserRegDto::createShared();
    dto->user->username = "user12344132321";
    dto->user->email = "dsadasdsadasda@gmail.com";
    dto->user->password = "123456";
    auto addedUserResponse = client->createUser(dto);
    OATPP_ASSERT(addedUserResponse != nullptr);
    OATPP_ASSERT(addedUserResponse->getStatusCode() == 200);

  }, std::chrono::minutes(10) /* test timeout */);

  /* wait all server threads finished */
  std::this_thread::sleep_for(std::chrono::seconds(1));
}