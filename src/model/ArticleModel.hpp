#ifndef ARTICLEMODEL_HPP
#define ARTICLEMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <string>

class ArticleModel {
  private:
    std::string timeTz(std::string &time);

  public:
    ArticleModel() {};
    oatpp::Object<ArticleDto> createArticle(std::string &userId, std::string &slug, std::string &title, std::string &description, std::string &body, std::string &createTime);
};

#endif // ARTICLEMODEL_HPP