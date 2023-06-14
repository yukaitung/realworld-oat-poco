#include "model/TagModel.hpp"
#include "helper/Database.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::RecordSet;
using Poco::Data::Statement;
using Poco::Exception;
using oatpp::web::protocol::http::Status;

TagCache TagModel::tagCache;

void TagCache::createTag(const std::pair<std::string, std::string> &tagNamePairTagId) {
  tagNamePairTagIdMap.insert({tagNamePairTagId.first, nullptr});
  tagIdPairTagNameMap.insert({tagNamePairTagId.second, nullptr});
  tagNamePairTagIdMap[tagNamePairTagId.first] = &tagIdPairTagNameMap.find(tagNamePairTagId.second)->first;
  tagIdPairTagNameMap[tagNamePairTagId.second] = &tagNamePairTagIdMap.find(tagNamePairTagId.first)->first;
}

bool TagCache::nameExist(const std::string name) {
  return tagNamePairTagIdMap.find(name) != tagNamePairTagIdMap.end();
}

std::string TagCache::getIdFromName(const std::string &tagName) {
  std::string id = "";
  if(tagNamePairTagIdMap.find(tagName) != tagNamePairTagIdMap.end()) {
    id = *tagNamePairTagIdMap[tagName];
  }
  return id;
}

std::string TagCache::getNameFromId(const std::string &id) {
  std::string name = "";
  if(tagIdPairTagNameMap.find(id) != tagIdPairTagNameMap.end()) {
    name = *tagIdPairTagNameMap[id];
  }
  return name;
}

void TagModel::initCache() {
  try {
    Session session(Database::getPool()->get());
    Statement select(session);
    select << "SELECT id, name FROM tags ORDER BY name ASC", now;
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();

    for(int i = 0; i < rowCount; i++) {
      tagCache.createTag({rs.value(1, i).toString(), rs.value(0, i).toString()});
    }
  }
  catch(Exception& exp) {
    OATPP_LOGE("TagModel", exp.displayText().c_str());
  }
}

bool TagModel::createTags(const oatpp::Vector<oatpp::String> &tags) {
  // Create tags if not exist
  std::vector<std::string> create;
  for(int i = 0; i < tags->size(); i++) {
    if(!tagCache.nameExist(tags->at(i))) {
      std::string temp = tags->at(i);
      create.push_back(temp);
    }
  }

  if(!create.empty()) {
    try {
      // Generate insert SQL
      Session session(Database::getPool()->get());
      Statement insert(session);
      insert << "INSERT INTO tags (name) VALUES ";
      for(int i = 0; i < create.size(); i++) {
        insert << "(?)", use(create[i]);
        if(i < (create.size() - 1)) {
          insert << ",";
        }
      }
      insert.execute();
      
      // Get id for new tags
      Statement select(session);
      select << "SELECT id, name FROM tags WHERE name IN(";
      for(int i = 0; i < create.size(); i++) {
        select << "?", use(create[i]);
        if(i < (create.size() - 1)) {
          select << ",";
        }
      }
      select << ")";
      select.execute();
      
      // Insert into cache
      RecordSet rs(select);
      size_t rowCount = rs.totalRowCount();
      for(int i = 0; i < rowCount; i++) {
        tagCache.createTag({rs.value(1, i).toString(), rs.value(0, i).toString()});
      }
    }
    catch(Exception& exp) {
      OATPP_LOGE("TagModel", exp.displayText().c_str());
      return false;
    }
  }
  return true;
}

std::string TagModel::getTagId(const std::string &tags) {
  return tagCache.getIdFromName(tags);
}

oatpp::Vector<oatpp::String> TagModel::getTagsId(const oatpp::Vector<oatpp::String> &tags) {
  oatpp::Vector<oatpp::String> id = oatpp::Vector<oatpp::String>::createShared();
  id->resize(tags->size());
  for(int i = 0; i < tags->size(); i++) {
    std::string temp = tagCache.getIdFromName(tags[i]);
    id[i] = temp;
  }
  return id;
}

oatpp::Vector<oatpp::String> TagModel::getTagsName(const std::vector<std::string> &tagsId) {
  oatpp::Vector<oatpp::String> tagName = oatpp::Vector<oatpp::String>::createShared();
  tagName->resize(tagsId.size());
  for(int i = 0; i < tagsId.size(); i++) {
    std::string temp = tagCache.getNameFromId(tagsId[i]);
    tagName[i] = temp;
  }

  // Sort name
  std::sort(tagName->begin(), tagName->end(), [] (const auto &lhs, const auto &rhs) {
    return lhs->compare(*rhs) < 0;
  });

  return tagName;
}

oatpp::Object<TagJsonDto> TagModel::getTags() {
  auto tags = TagJsonDto::createShared();
  tags->tags = {};
  tags->tags->resize(tagCache.getSize());

  auto it = tagCache.getTagNameBegin();
  int i = 0;
  while (it != tagCache.getTagNameEnd())
  {
    std::string name = it->first;
    tags->tags->at(i++) = name;
    it++;
  }
  return tags;
}