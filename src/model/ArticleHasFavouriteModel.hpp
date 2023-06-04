#ifndef ARTICLEHASFAVOURITEMODEL_HPP
#define ARTICLEHASFAVOURITEMODEL_HPP

#include <string>
#include <utility>

class ArticleHasFavouriteModel {
  public:
    ArticleHasFavouriteModel() {};
    bool favouriteArticle(std::string &articleId, std::string &userId);
    bool unfavouriteArticle(std::string &articleId, std::string &userId);
    std::pair<unsigned int, bool> getArticlefavouriteData(std::string &articleId, std::string &userId);
};

#endif // ARTICLEHASFAVOURITEMODEL_HPP