#ifndef COMMENTMODEL_HPP
#define COMMENTMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <string>

class CommentModel {
  public:
    CommentModel() {};
    
    std::string timeTz(std::string &time);
    oatpp::Object<CommentDto> createComment(std::string &userId, std::string &articleId, std::string &body, std::string &createTime);
};

#endif // COMMENTMODEL_HPP