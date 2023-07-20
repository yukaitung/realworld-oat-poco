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
  bool m_streaming;

  oatpp::String loadFromFile(const char* fileName);
  void cacheResource(const char* fileName);
  bool hasEnding(std::string fullString, std::string const &ending) const;

  class ReadCallback : public oatpp::data::stream::ReadCallback {
   private:
    oatpp::String m_file;
    oatpp::data::stream::FileInputStream m_stream;

   public:
    ReadCallback(const oatpp::String& file);
    oatpp::v_io_size read(void *buffer, v_buff_size count, oatpp::async::Action& action) override;
  };

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
  static std::shared_ptr<FrontEndService> loadResources(const oatpp::String& resDir) {
    auto res = std::make_shared<FrontEndService>(resDir);
    res->cacheResource("index.html");
    res->cacheResource("qtloader.js");
    res->cacheResource("qtlogo.svg");
    res->cacheResource("Realworld-Qt-Qml.js");
    res->cacheResource("Realworld-Qt-Qml.wasm");
    return res;
  }

  /**
   * Stream Swagger-UI resources directly from disk.
   * @param resDir - directory containing swagger-ui resources.
   * @return - `std::shared_ptr` to Resources.
   */
  static std::shared_ptr<FrontEndService> streamResources(const oatpp::String& resDir) {
    auto res = std::make_shared<FrontEndService>(resDir, true);

    return res;
  }

  /**
   * Get cached resource by filename.
   * @param filename - name of the resource file.
   * @return - &id:oatpp::String; containing resource binary data.
   */
  oatpp::String getResource(const oatpp::String& filename);

  /**
   * Get streamed resource by filename.
   * @param filename - name of the resource file.
   * @return - `std::shared_ptr` to &id:oatpp::data::stream::ReadCallback; containing resource binary data stream."
   */
  std::shared_ptr<ReadCallback> getResourceStream(const oatpp::String& filename);

  /**
   * Returns true if this is a streaming ressource instance.
   * @return
   */
  bool isStreaming() {
    return m_streaming;
  }

  /**
   * Returns the MIME type for a given filename
   * @param filename to return the MIME type
   * @return a MIME type
   */
  std::string getMimeType(const std::string &filename) const;
};

#endif // FRONTENDSERVICE_HPP
