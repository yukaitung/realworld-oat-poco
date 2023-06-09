#ifndef TAGMODEL_HPP
#define TAGMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iterator>

class TagCache {
  private:
    std::map<std::string, std::string> tagNamePairTagIdMap;

  public:
    TagCache() {};
    void createTag(const std::pair<std::string, std::string> &tagNamePairTagId);
    std::string getIdFromName(const std::string &tagNamePairTagId);
    bool nameExist(const std::string name); 
    const std::map<std::string, std::string>::iterator getTagNameBegin() {return tagNamePairTagIdMap.begin();};
    const std::map<std::string, std::string>::iterator getTagNameEnd() {return tagNamePairTagIdMap.end();};
    const size_t getSize() {return tagNamePairTagIdMap.size();};
};
class TagModel {
  private:
    static TagCache tagCache;

  public:
    TagModel() {};
    static void initCache();
    bool createTags(std::vector<std::string> tags);
    std::vector<std::string> getTagsId(std::vector<std::string> tags);
    oatpp::Object<TagJsonDto> getTags();
};

#endif // TAGMODEL_HPP