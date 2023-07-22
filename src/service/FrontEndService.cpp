#include "service/FrontEndService.hpp"
#include <stdio.h>
#include <fstream>
#include <filesystem>

oatpp::String FrontEndService::loadFromFile(const char* fileName) {
  auto fullFilename = m_resDir + fileName;
  
  std::ifstream file (fullFilename->c_str(), std::ios::in|std::ios::binary|std::ios::ate);
  
  if (file.is_open()) {   
    auto result = oatpp::String((v_int32) file.tellg());
    file.seekg(0, std::ios::beg);
    file.read((char*)result->data(), result->size());
    file.close();
    return result;
  }
  
  OATPP_LOGE("FrontEndService", ":%s(): Cannot load file %s", __func__, fullFilename->c_str());
  throw std::runtime_error("[FrontEndService]: Can't load file. Please make sure you specified full path to /front folder");
}
  
void FrontEndService::cacheResource(const char* fileName) {
  m_resources[fileName] = loadFromFile(fileName);
}

bool FrontEndService::hasEnding(std::string fullString, std::string const &ending) const {
  std::transform(fullString.begin(), fullString.end(), fullString.begin(),
                  [](unsigned char c) { return std::tolower(c); });
  if (fullString.length() >= ending.length()) {
      return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  } else {
      return false;
  }
}
  
FrontEndService::FrontEndService(const oatpp::String& resDir, bool streaming) {
  if(!resDir || resDir->size() == 0) {
    throw std::runtime_error("[FrontEndService]: Invalid resDir path. Please specify full path to /front folder");
  }
  
  m_resDir = resDir;
  if(m_resDir->data()[m_resDir->size() - 1] != '/') {
    m_resDir = m_resDir + "/";
  }
}

std::shared_ptr<FrontEndService> FrontEndService::loadResources(const oatpp::String& resDir) {
  auto res = std::make_shared<FrontEndService>(resDir);

  const std::filesystem::path fs{resDir};
  if(!std::filesystem::exists(fs)) {
    OATPP_LOGD("FrontendService", "Frontend resources not found.");
    return res;
  }
  for (auto const& dir_entry : std::filesystem::directory_iterator{fs}) 
  {
    OATPP_LOGD("FrontendService", "Found and cache file %s", dir_entry.path().filename().c_str());
    res->cacheResource(dir_entry.path().filename().c_str());
  }
  return res;
}
  
oatpp::String FrontEndService::getResource(const oatpp::String& filename) {

  auto it = m_resources.find(filename);
  if(it != m_resources.end()) {
    return it->second;
  }
  throw std::runtime_error("[FrontEndService]: Resource file not found.");
}


std::string FrontEndService::getMimeType(const std::string &filename) const {
    if (hasEnding(filename, ".html")) return "text/html";
    if (hasEnding(filename, ".jpg")) return "image/jpeg";
    if (hasEnding(filename, ".jpeg")) return "image/jpeg";
    if (hasEnding(filename, ".png")) return "image/png";
    if (hasEnding(filename, ".gif")) return "image/gif";
    if (hasEnding(filename, ".css")) return "text/css";
    if (hasEnding(filename, ".js")) return "text/javascript";
    if (hasEnding(filename, ".xml")) return "text/xml";
    return "text/plain";
}