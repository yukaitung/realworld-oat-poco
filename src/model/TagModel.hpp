#ifndef TAGMODEL_HPP
#define TAGMODEL_HPP

#include "dto/ArticleDto.hpp"

class TagModel {
  public:
    TagModel() {};
    oatpp::Object<TagJsonDto> getTags();
};

#endif // TAGMODEL_HPP