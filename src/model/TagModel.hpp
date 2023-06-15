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
    std::map<std::string, const std::string*> tagNamePairTagIdMap;
    std::map<std::string, const std::string*> tagIdPairTagNameMap;

  public:
    TagCache() {};
    void createTag(const std::pair<std::string, std::string> &tagNamePairTagId);
    std::string getIdFromName(const std::string &tagName);
    std::string getNameFromId(const std::string &id);
    bool nameExist(const std::string name); 
    const std::map<std::string, const std::string*>::iterator getTagNameBegin() {return tagNamePairTagIdMap.begin();};
    const std::map<std::string, const std::string*>::iterator getTagNameEnd() {return tagNamePairTagIdMap.end();};
    const size_t getSize() {return tagNamePairTagIdMap.size();};
};

class TagModel {
  private:
    static TagCache tagCache;

  public:
    TagModel() {};
    static void InitCache();
    bool createTags(const oatpp::Vector<oatpp::String> &tags);
    std::string getTagId(const std::string &tags);
    oatpp::Vector<oatpp::String> getTagsId(const oatpp::Vector<oatpp::String> &tags);
    oatpp::Vector<oatpp::String> getTagsName(const std::vector<std::string> &tagsId);
    oatpp::Object<TagJsonDto> getTags();
};

#endif // TAGMODEL_HPP