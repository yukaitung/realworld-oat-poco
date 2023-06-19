#ifndef USERCONTROLLERTEST_HPP
#define USERCONTROLLERTEST_HPP

#include "oatpp-test/UnitTest.hpp"
#include <string>

class UserControllerTest : public oatpp::test::UnitTest {
  private:
    std::string userToken;

  public:
    UserControllerTest() : oatpp::test::UnitTest("TEST[UserControllerTest]") {}
    void onRun() override;
};

#endif // USERCONTROLLERTEST_HPP
