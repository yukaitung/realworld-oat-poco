#ifndef USERCONTROLLERTEST_HPP
#define USERCONTROLLERTEST_HPP

#include "oatpp-test/UnitTest.hpp"

class UserControllerTest : public oatpp::test::UnitTest {
  public:
    UserControllerTest() : oatpp::test::UnitTest("TEST[UserControllerTest]") {}
    void onRun() override;
};

#endif // USERCONTROLLERTEST_HPP
