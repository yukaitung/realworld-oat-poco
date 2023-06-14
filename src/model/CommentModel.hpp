#ifndef COMMENTMODEL_HPP
#define COMMENTMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <string>
#include <utility>

class CommentModel {
  public:
    CommentModel() {};
    
    std::string timeTz(std::string &time);
    oatpp::Object<CommentDto> createComment(std::string &userId, std::string &articleId, std::string &body, std::string &createTime);
    std::pair<oatpp::Vector<oatpp::Object<CommentDto>>, std::vector<std::string>> getComments(std::string &articleId);
    std::string getCommentAuthorId(std::string &commentId);
    bool deleteComment(std::string &commentId);
};

#endif // COMMENTMODEL_HPP