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

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::RecordSet;
using Poco::Data::Statement;
using Poco::Exception;
using oatpp::web::protocol::http::Status;

TagCache TagModel::tagCache;

void TagCache::createTag(const std::pair<std::string, std::string> &tagNamePairTagId) {
  tagNamePairTagIdMap.insert({tagNamePairTagId.first, tagNamePairTagId.second});
}

bool TagCache::nameExist(const std::string name) {
  return tagNamePairTagIdMap.find(name) != tagNamePairTagIdMap.end();
}

std::string TagCache::getIdFromName(const std::string &tagNamePairTagId) {
  std::string name = "";
  if(tagNamePairTagIdMap.find(tagNamePairTagId) != tagNamePairTagIdMap.end()) {
    name = tagNamePairTagIdMap[tagNamePairTagId];
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

bool TagModel::createTags(std::vector<std::string> tags) {
  // Create tags if not exist
  auto it = tags.begin();
  while (it != tags.end()) {
    if(tagCache.nameExist(*it)) {
      // Tag exist
      it = tags.erase(it);    
    }
    else {
      it++;
    }
  }

  if(!tags.empty()) {
    try {
      // Generate insert SQL
      Session session(Database::getPool()->get());
      Statement insert(session);
      insert << "INSERT INTO tags (name) VALUES ";
      for(int i = 0; i < tags.size(); i++) {
        insert << "(?)", use(tags[i]);
        if(i < (tags.size() - 1)) {
          insert << ",";
        }
      }
      insert.execute();
      
      // Get id for new tags
      Statement select(session);
      select << "SELECT id, name FROM tags WHERE name IN(";
      for(int i = 0; i < tags.size(); i++) {
        select << "?", use(tags[i]);
        if(i < (tags.size() - 1)) {
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

std::vector<std::string> TagModel::getTagsId(std::vector<std::string> tags) {
  std::vector<std::string> id(tags.size());
  for(int i = 0; i < tags.size(); i++) {
    id[i] = tagCache.getIdFromName(tags[i]);
  }
  return id;
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