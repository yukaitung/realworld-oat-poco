#ifndef USERCONTROLLERTEST_HPP
#define USERCONTROLLERTEST_HPP

#include "oatpp-test/UnitTest.hpp"
#include <string>

class UserControllerTest : public oatpp::test::UnitTest {
  private:
    inline static const std::string USER_NAME = "輕心街";
    inline static const std::string USER_EMAIL = "handsomeguy@example.com";
    inline static const std::string USER_PASSWORD = "123456";
    inline static const std::string USER_BIO = "測試";
    inline static const std::string USER_IMAGE = "www.example.com";
    static std::string USER_TOKEN;

  public:
    UserControllerTest() : oatpp::test::UnitTest("TEST[UserControllerTest]") {}
    void onRun() override;
};

#endif // USERCONTROLLERTEST_HPP
