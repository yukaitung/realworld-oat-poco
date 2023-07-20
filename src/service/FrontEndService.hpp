#ifndef FRONTENDSERVICE_HPP
#define FRONTENDSERVICE_HPP

#include "oatpp/core/Types.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"
#include "oatpp/core/data/stream/FileStream.hpp"

#include <unordered_map>

class FrontEndService {

private:
  oatpp::String m_resDir;
  std::unordered_map<oatpp::String, oatpp::String> m_resources;

  oatpp::String loadFromFile(const char* fileName);
  void cacheResource(const char* fileName);
  bool hasEnding(std::string fullString, std::string const &ending) const;
  
public:
  /**
   * Constructor.
   * @param resDir - directory containing swagger-ui resources.
   */
  FrontEndService(const oatpp::String& resDir, bool streaming = false);

  /**
   * Load and cache Swagger-UI resources.
   * @param resDir - directory containing swagger-ui resources.
   * @return - `std::shared_ptr` to Resources.
   */
  static std::shared_ptr<FrontEndService> loadResources(const oatpp::String& resDir);

  /**
   * Get cached resource by filename.
   * @param filename - name of the resource file.
   * @return - &id:oatpp::String; containing resource binary data.
   */
  oatpp::String getResource(const oatpp::String& filename);

  /**
   * Returns the MIME type for a given filename
   * @param filename to return the MIME type
   * @return a MIME type
   */
  std::string getMimeType(const std::string &filename) const;
};

#endif // FRONTENDSERVICE_HPP
