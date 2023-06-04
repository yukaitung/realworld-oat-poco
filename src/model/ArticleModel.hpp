#ifndef ARTICLEMODEL_HPP
#define ARTICLEMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <string>
#include <utility>

class ArticleModel {
  private:
    std::string timeTz(std::string &time);

  public:
    enum GetArticleEnum {Article, ArticleId, AuthorId};
    
    ArticleModel() {};
    oatpp::Object<ArticleDto> createArticle(std::string &userId, std::string &slug, std::string &title, std::string &description, std::string &body, std::string &createTime);
    std::tuple<oatpp::Object<ArticleDto>, std::string, std::string> getArticle(std::string &slug);
};

#endif // ARTICLEMODEL_HPP