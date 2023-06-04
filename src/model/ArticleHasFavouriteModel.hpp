#ifndef ARTICLEHASFAVOURITEMODEL_HPP
#define ARTICLEHASFAVOURITEMODEL_HPP

#include <string>
#include <utility>

class ArticleHasFavouriteModel {
  public:
    ArticleHasFavouriteModel() {};
    bool favouriteArticle(std::string &articleId, std::string &userId);
    bool unfavouriteArticle(std::string &articleId, std::string &userId);
    /**
    * This method is to obtain favourite count and see if the user favourited the article.
    * @param articleId The article id.
    * @param userId The article id.
    * @return std::pair<unsigned int, bool> This returns favourite count and the user favourited the article or not. The favourite count becomes -1 if an error occured.
    */
    std::pair<unsigned int, bool> getArticlefavouriteData(std::string &articleId, std::string &userId);
};

#endif // ARTICLEHASFAVOURITEMODEL_HPP