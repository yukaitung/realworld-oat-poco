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

std::map<std::string, std::string> TagModel::tagCache;

void TagModel::initCache() {
  try {
    Session session(Database::getPool()->get());
    Statement select(session);
    select << "SELECT id, name FROM tags ORDER BY name ASC", now;
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();

    for(int i = 0; i < rowCount; i++) {
      tagCache.insert({rs.value(0, i).toString(), rs.value(1, i).toString()});
    }
  }
  catch(Exception& exp) {
    OATPP_LOGE("TagModel", exp.displayText().c_str());
  }
}

oatpp::Object<TagJsonDto> TagModel::getTags() {
  auto tags = TagJsonDto::createShared();
  tags->tags = {};
  tags->tags->resize(tagCache.size());

  auto it = tagCache.begin();
  int i = 0;
  while (it != tagCache.end())
  {
    std::string name = it->first;
    tags->tags->at(i++) = name;
    it++;
  }
  return tags;
}