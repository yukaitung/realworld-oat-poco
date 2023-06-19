#ifndef PROFILECONTROLLERTEST_HPP
#define PROFILECONTROLLERTEST_HPP

#include "oatpp-test/UnitTest.hpp"
#include <string>

class ProfileControllerTest : public oatpp::test::UnitTest {
  private:
    std::string userToken;

  public:
    ProfileControllerTest() : oatpp::test::UnitTest("TEST[ProfileControllerTest]") {}
    void onRun() override;
};


#endif // PROFILECONTROLLERTEST_HPP