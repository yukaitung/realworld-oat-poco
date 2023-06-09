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
    /**
    * This method is to create an article into database.
    * @param userId The author user id of article.
    * @param slug The slug of article.
    * @param title The title of article.
    * @param description The description of article.
    * @param tags The tags of article.
    * @param body The content of article.
    * @param createTime The create time and update time of article.
    * @return oatpp::Object<ArticleDto> This returns article object. Remember to append data for author profile.
    */
    oatpp::Object<ArticleDto> createArticle(std::string &userId, std::string &slug, std::string &title, std::string &description, std::string &body, std::vector<std::string> tags, std::string &createTime);
    /**
    * This method is to fetch an article from database.
    * @param slug The slug of article.
    * @return std::tuple<oatpp::Object<ArticleDto>, std::string, std::string> This returns article object, article id, author user id. Remember to append data for favourite and author profile.
    */
    std::tuple<oatpp::Object<ArticleDto>, std::string, std::string> getArticle(std::string &slug);
};

#endif // ARTICLEMODEL_HPP