#ifndef ARTICLECONTROLLERTEST_HPP
#define ARTICLECONTROLLERTEST_HPP

#include "oatpp-test/UnitTest.hpp"
#include <string>

class ArticleControllerTest : public oatpp::test::UnitTest {
  private:
    std::string userToken;
    std::string currentSlug;
    int currentCommentId = 0;

  public:
    ArticleControllerTest() : oatpp::test::UnitTest("TEST[ArticleControllerTest]") {}
    void onRun() override;
};

#endif // ARTICLECONTROLLERTEST_HPP